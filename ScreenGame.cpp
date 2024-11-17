/**
 * ScreenGame.cpp source file
 */

// Game classes
#include "TheApp.h"
#include "ScreenGame.h"

ScreenGame::ScreenGame(void)
{
}

ScreenGame::~ScreenGame(void)
{
}

void ScreenGame::OnRelease(void)
{
	Clear();
}

void ScreenGame::Clear(void)
{
	// ball objects exist
	if (!m_ArrayBallObjects.empty())
	{
		m_ArrayBallObjects.clear();
	}
}

void ScreenGame::Initialize()
{
	Clear();

	// Create game objects.
	CreateBalls();
	CreatePads();
}

void ScreenGame::Create(CTheApp* app)
{
	//TODO: USE APP TO GET WINDOW WIDTH AND HEIGHT

	m_TheApp = app;
	m_ArrayBallObjects.reserve(MAX_BALLS);
}

void ScreenGame::CreateBalls()
{
	int windowWidth = m_TheApp->GetWindowWidth();
	int windowHeight = m_TheApp->GetWindowHeight();

	int playAreaBorderHeight = GetGameAreaBorderHeight(windowHeight);
	int padWidth = GetObjectPlayPadWidth(windowHeight);
	float verticalMargin = windowHeight / 2.0f / 4.0f;

	// BALL 1

	ObjectBall ball_1;
	float startPosY = windowHeight / 2.0f - verticalMargin;

	ball_1.Initialize(m_TheApp, playAreaBorderHeight, padWidth);
	ball_1.SetStartDirection(ObjectBall::Right, startPosY);

	m_ArrayBallObjects.push_back(ball_1);

	// BALL 2

	ObjectBall ball_2;
	startPosY = windowHeight / 2.0f + verticalMargin;

	ball_2.Initialize(m_TheApp, playAreaBorderHeight, padWidth);
	ball_2.SetStartDirection(ObjectBall::Left, startPosY);

	m_ArrayBallObjects.push_back(ball_2);
}

void ScreenGame::CreatePads()
{
	int windowWidth = m_TheApp->GetWindowWidth();
	int windowHeight = m_TheApp->GetWindowHeight();

	int playAreaBorderHeight = GetGameAreaBorderHeight(windowHeight);
	int padWidth = GetObjectPlayPadWidth(windowHeight);
	int padHeight = GetObjectPlayPadHeight(windowHeight);
	double padCornerRadius = GetObjectPlayPadRadius(windowHeight);

	m_ObjectPadLeft.Initialize(windowWidth, windowHeight, playAreaBorderHeight,
		padWidth, padHeight, padCornerRadius, ObjectPad::Left);

	m_ObjectPadRight.Initialize(windowWidth, windowHeight, playAreaBorderHeight,
		padWidth, padHeight, padCornerRadius, ObjectPad::Right);
}

BOOL ScreenGame::UpdateKeyboard(void)
{
	// 0x01 = single key press
	// 0x8000 = continuous

	// UP ARROW
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
	}
	// DOWN ARROW
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
	}

	return TRUE;
}

void ScreenGame::UpdateScreen(float deltaTime)
{
	// Ball objects exist.
	if (!m_ArrayBallObjects.empty())
	{
		// Update ball objects.
		for (std::vector<ObjectBall>::iterator it = m_ArrayBallObjects.begin(); it != m_ArrayBallObjects.end(); ++it)
		{
			ObjectBall& objectBall = *it;
			objectBall.Update(deltaTime, &m_ObjectPadLeft, &m_ObjectPadRight);
		}
	}

	// Update pad objects.
	m_ObjectPadLeft.Update(deltaTime, &m_ArrayBallObjects);
	m_ObjectPadRight.Update(deltaTime, &m_ArrayBallObjects);
}

void ScreenGame::RenderScreen(std::string windowName)
{
	int windowWidth = m_TheApp->GetWindowWidth();
	int windowHeight = m_TheApp->GetWindowHeight();

	BLImage img(windowWidth, windowHeight, BL_FORMAT_PRGB32);
	BLContext ctx(img);

	// Render plain background color (empty/black).
	ctx.setCompOp(BL_COMP_OP_CLEAR);
	ctx.fillAll();

	// Start rendering other elements on the screen.
	ctx.setCompOp(BL_COMP_OP_OVERLAY);

	// Render ball objects.
	if (!m_ArrayBallObjects.empty())
	{
		for (std::vector<ObjectBall>::iterator it = m_ArrayBallObjects.begin(); it != m_ArrayBallObjects.end(); ++it)
		{
			it->Render(&ctx, windowWidth, windowHeight);
		}
	}

	// Render pad objects.
	m_ObjectPadLeft.Render(&ctx, windowWidth, windowHeight);
	m_ObjectPadRight.Render(&ctx, windowWidth, windowHeight);

	RenderGameAreaBorderTop(&ctx);
	RenderGameAreaBorderBottom(&ctx);

	// Finish Blend2d render commands.
	ctx.end();

	BLImageData imgData;
	img.getData(&imgData);

	// Show our image inside the window.
	Mat cvImg(img.height(), img.width(), CV_8UC4, imgData.pixelData);
	imshow(windowName, cvImg);
}

void ScreenGame::RenderGameAreaBorderTop(BLContext* ctx)
{
	int windowWidth = m_TheApp->GetWindowWidth();
	int windowHeight = m_TheApp->GetWindowHeight();

	BLRgba32 color(COLOR_GAMEAREA_BORDER);

	int height = GetGameAreaBorderHeight(windowHeight);

	float topLeftCornerX = 0;
	float topLeftCornerY = 0;

	ctx->setFillStyle(color);
	ctx->fillRoundRect(topLeftCornerX, topLeftCornerY, windowWidth, height, 0);
}

void ScreenGame::RenderGameAreaBorderBottom(BLContext* ctx)
{
	int windowWidth = m_TheApp->GetWindowWidth();
	int windowHeight = m_TheApp->GetWindowHeight();

	BLRgba32 color(COLOR_GAMEAREA_BORDER);

	int height = GetGameAreaBorderHeight(windowHeight);

	float topLeftCornerX = 0;
	float topLeftCornerY = windowHeight - height;

	ctx->setFillStyle(color);
	ctx->fillRoundRect(topLeftCornerX, topLeftCornerY, windowWidth, height, 0);
}

/*

// Example: Gradient color fill (the whole screen)
BLGradient linear(BLLinearGradientValues(0, 0, 0, img.height()));
linear.addStop(0.0, BLRgba32(0xFFFFFFFF));
linear.addStop(0.5, BLRgba32(0xFF5FAFDF));
linear.addStop(1.0, BLRgba32(0xFF2F5FDF));
ctx.setFillStyle(linear);
ctx.setCompOp(BL_COMP_OP_OVERLAY);
ctx.fillAll();

// Example: Circle with color fill
BLRgba32 color(0xFFFFFFFF);
ctx.setFillStyle(color);
ctx.setCompOp(BL_COMP_OP_OVERLAY);
ctx.fillRoundRect(0.0, 0.0, 100.0, 100.0, 50.0);
ctx.fillRoundRect(400.0, 0.0, 100.0, 100.0, 50.0);

// Example: Drawing rectangle with OpenCV
Scalar color = GetObjectPlayPadColor();
int width = GetObjectPlayPadWidth(windowHeight);
int height = GetObjectPlayPadHeight(windowHeight);
int moveOffsetY = 0;
int currentPosX = windowWidth - width;
int currentPosY = windowHeight / 2 + moveOffsetY;
Point p1(currentPosX, currentPosY - height / 2);
Point p2(currentPosX + width, currentPosY + height / 2);
rectangle(*image, p1, p2, color, FILLED, LINE_8);

*/
