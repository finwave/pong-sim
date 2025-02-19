/**
 * IApplication.cpp source file
 * 
 * An abstract class implements basic
 * features of game application, including
 * window handling etc.
 */

#include "IApplication.h"

IApplication::IApplication(void)
{
	// initialise the members

	m_WindowHandle = NULL;
	m_iWindowWidth = 0;
	m_iWindowHeight = 0;

	m_bExitApp = FALSE;
	m_bActive = FALSE;

	// seed the random number generator
	::srand( ::GetTickCount64() );
}

IApplication::~IApplication(void)
{
}

void IApplication::Create(BOOL isFullScreen)
{
	m_bFullScreen = isFullScreen;

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (m_bFullScreen)
	{
		m_iWindowWidth = screenWidth;
		m_iWindowHeight = screenHeight;
	}
	else
	{
		m_iWindowWidth = screenWidth / 1.4f;
		m_iWindowHeight = screenHeight / 1.4f;
	}

	SetActive(TRUE);

	// initialise the timer
	m_Timer.Create();

	// call pure virtual OnCreate
	OnCreate();
}

void IApplication::Release(void)
{
	// call pure virtual OnRelease
	OnRelease();

	// release the timer
	m_Timer.Release();
}

void IApplication::Close(void)
{
	m_bExitApp = TRUE;
}

void IApplication::SetActive(BOOL bSet)
{
	m_bActive = bSet;
	m_Timer.BeginTimer();
}

int IApplication::Run(void)
{
	MSG		msg;
	BOOL	bGotMessage = FALSE;

	::memset(&msg, 0, sizeof(MSG));

	do
	{
		bGotMessage = FALSE;

		if (m_WindowHandle != NULL)
		{
			if (IsActive())
			{
				// we are active and want to update screen as fast as
				// possible. Use PeekMessage to get the windows messages.
				// PeekMessage returns immediately if there are no
				// messages pending in windows message queue.
				bGotMessage = ::PeekMessage(&msg, m_WindowHandle, 0, 0, PM_REMOVE);
			}
			else
			{
				bGotMessage = ::GetMessage(&msg, m_WindowHandle, 0, 0);
			}
		}

		if (bGotMessage)
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		DoFlip();
	}
	while (m_bExitApp == FALSE);

	Release();
	return 0;
}

void IApplication::DoFlip(void)
{
	if(!IsActive())
	{
		return;
	}

	// compute the frame time
	m_Timer.EndTimer();
	m_Timer.BeginTimer();

	// call pure virtual OnFlip
	OnFlip();
}

int IApplication::RandInt(int iLow, int iHigh)
{
	return (::rand() % ((iHigh + 1) - iLow) + iLow);
}

float IApplication::RandFloat(float fLow, float fHigh)
{
	return ((fHigh - fLow) * (float)(::rand() / (float)RAND_MAX)) + fLow;
}

LPCWSTR IApplication::StringToWideStr(std::string str)
{
	// Initializing an object of wstring
	std::wstring temp = std::wstring(str.begin(), str.end());

	// Applying c_str() method on temp
	LPCWSTR wideString = temp.c_str();

	return wideString;
}
