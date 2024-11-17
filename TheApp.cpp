/** 
 * TheApp.cpp source file
 */

#include "TheApp.h"

CTheApp::CTheApp(void)
{
	// initialise member variables

	m_iRandomSeed = 0;
	m_fTargetDeltaTime = 1.0f / FRAME_TIME;
	m_fAccumulatedDeltaTime = 0;
}

CTheApp::~CTheApp(void)
{
}

HRESULT CTheApp::OnCreate(void)
{
	HRESULT hres = S_OK;
	
	// generate random seed
	this->m_iRandomSeed = static_cast<unsigned int>(time(NULL));

	if (m_bFullScreen)
	{
		// create OpenCV screen (full screen)
		namedWindow(WINDOW_NAME, WINDOW_NORMAL);
		setWindowProperty(WINDOW_NAME, WND_PROP_FULLSCREEN, WINDOW_FULLSCREEN);
	}
	else
	{
		// create OpenCV screen (window)
		namedWindow(WINDOW_NAME, WINDOW_AUTOSIZE);
	}

	SwitchState(LoadingStart);

	return hres;
}

void CTheApp::OnRelease(void)
{
}

void CTheApp::OnFlip(void)
{
	float deltaTime = GetFrameTime();
	m_fAccumulatedDeltaTime += deltaTime;

	// get window handle
	if (!GetWindowHandle())
	{
		Close();
		return;
	}

	// update keyboard
	if (!UpdateKeyboard())
	{
		Close();
		return;
	}

	// update scene
	UpdateScene(deltaTime);

	// render scene
	if (m_fAccumulatedDeltaTime >= m_fTargetDeltaTime)
	{
		int offset = m_fAccumulatedDeltaTime - m_fTargetDeltaTime;
		m_fAccumulatedDeltaTime = offset;

		RenderScene();
	}
}

BOOL CTheApp::GetWindowHandle(void)
{
	if (m_WindowHandle == NULL)
	{
		m_WindowHandle = FindWindowA(NULL, WINDOW_NAME.c_str());
	}
	else if (FindWindowA(NULL, WINDOW_NAME.c_str()) == NULL)
	{
		return false;
	}

	return true;
}

void CTheApp::UpdateScene(float deltaTime)
{
	if ((m_State == StateMenu) || (m_State == StateGame))
	{
		m_SoundManager.Update(deltaTime);
	}

	switch (m_State)
	{
	case LoadingStart:
		if (m_iLoadingScreenWaitFrames <= 0)
		{
			SwitchState(LoadingResources);
		}
		break;
	case LoadingResources:
		m_Resources.Initialize();
		SwitchState(LoadingSound);
		break;
	case LoadingSound:
		m_SoundManager.Initialize(&m_Resources);
		SwitchState(LoadingGameScreens);
		break;
	case LoadingGameScreens:
		m_ScreenMenu.Create(this);
		m_ScreenGame.Create(this);
		SwitchState(StateMenu);
		break;
	case StateMenu:
		m_ScreenMenu.UpdateScreen(deltaTime);
		break;
	case StateGame:
		m_ScreenGame.UpdateScreen(deltaTime);
		break;
	}
}

BOOL CTheApp::UpdateKeyboard(void)
{
	// 0x01 = single key press
	// 0x8000 = continuous

	switch (m_State)
	{
	case StateMenu:
		m_ScreenMenu.UpdateKeyboard();
		break;
	case StateGame:
		m_ScreenGame.UpdateKeyboard();
		break;
	}

	// ESC
	if (GetAsyncKeyState(VK_ESCAPE) & 0x01)
	{
		switch (m_State)
		{
		// Exit game
		case StateMenu:
			return FALSE;
		// Go to menu screen.
		case StateGame:
			SwitchState(StateMenu);
			break;
		}
	}
	// RETURN
	else if (GetAsyncKeyState(VK_RETURN) & 0x01)
	{
		switch (m_State)
		{
		// Go to game screen.
		case StateMenu:
			SwitchState(StateGame);
			break;
		}
	}
	// M (toggle music)
	else if (GetAsyncKeyState('M') & 0x01)
	{
		switch (m_State)
		{
		case StateMenu:
		case StateGame:
			m_SoundManager.PlaySoundToggle();
			m_SoundManager.ToggleMusic();
			break;
		}
	}

	return TRUE;
}

void CTheApp::SwitchState(EState nextState)
{
	// Release the previous game screen.
	switch (m_State)
	{
	case StateMenu:
		m_ScreenMenu.OnRelease();
		break;
	case StateGame:
		m_ScreenGame.OnRelease();
		break;
	}

	// Prepare the next game screen.
	switch (nextState)
	{
	case StateMenu:
		m_ScreenMenu.Initialize();
		m_SoundManager.PlayMusicMenu();
		break;
	case StateGame:
		m_ScreenGame.Initialize();
		m_SoundManager.PlaySoundGameStart();
		m_SoundManager.PlayMusicGame();
		break;
	}

	m_State = nextState;
}

void CTheApp::RenderScene(void)
{
	switch (m_State)
	{
	case LoadingStart:
		RenderLoading(WINDOW_NAME);
		m_iLoadingScreenWaitFrames--;
		break;
	case LoadingResources:
	case LoadingSound:
	case LoadingGameScreens:
		RenderLoading(WINDOW_NAME);
		break;
	case StateMenu:
		m_ScreenMenu.RenderScreen(WINDOW_NAME);
		break;
	case StateGame:
		m_ScreenGame.RenderScreen(WINDOW_NAME);
		break;
	}
}

void CTheApp::RenderLoading(string windowName)
{
	int windowWidth = GetWindowWidth();
	int windowHeight = GetWindowHeight();

	BLImage img(windowWidth, windowHeight, BL_FORMAT_PRGB32);
	BLContext ctx(img);

	// Render plain background color (empty/black).
	ctx.setCompOp(BL_COMP_OP_CLEAR);
	ctx.fillAll();

	// Finish Blend2d render commands.
	ctx.end();

	BLImageData imgData;
	img.getData(&imgData);

	// Show our image inside the window.
	Mat cvImg(img.height(), img.width(), CV_8UC4, imgData.pixelData);
	imshow(windowName, cvImg);
}

string CTheApp::IntToString(int iNumber)
{
	string sNumber;
	stringstream out;
	
	out << iNumber;
	sNumber = out.str();

	return sNumber;
}

char* CTheApp::IntToChar(int iNumber)
{
	char* sReturn = NULL;
	sReturn = new char[128];
	
	sprintf_s(sReturn,128, "%d", iNumber);

	return sReturn;
}

string CTheApp::IntToHex(int iNumber)
{
	stringstream ss;
    ss << hex << iNumber;
    return ss.str();
}

wstring CTheApp::StringToWchar(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	wstring r(buf);
	delete [] buf;
	return r;
}

string CTheApp::WcharToString(const std::wstring& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	char* buf = new char[len];
	WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, buf, len, 0, 0);
	string r(buf);
	delete [] buf;
	return r;
}
