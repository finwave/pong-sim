/**
 * ScreenGame.h header file
 */

#pragma once

#ifndef __SCREENGAME_H__
#define __SCREENGAME_H__

#pragma warning(disable : 4995)

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// Blend2d
#include <blend2d.h>

// Game classes
#include "ObjectBall.h"
#include "ObjectPad.h"

using namespace cv;

// Game classes (forward declaration)
class CTheApp;

class ScreenGame
{
public:

	ScreenGame(void);
	virtual ~ScreenGame(void);

	/**
	 * OnRelease
	 * Place to release all the assets of this game screen.
	 */
	void OnRelease(void);

	void Initialize();
	void Create(CTheApp* app);

	BOOL UpdateKeyboard(void);
	void UpdateScreen(float deltaTime);
	void RenderScreen(std::string windowName);

	void SetSimulationGameMode(void);
	void SetPlayerGameMode(void);

private:

	CTheApp* m_TheApp;

	void Clear(void);

	// PLAY AREA

	inline int GetGameAreaBorderHeight(int windowHeight) { return windowHeight / 30; }

	void RenderGameAreaBorderTop(BLContext* ctx);
	void RenderGameAreaBorderBottom(BLContext* ctx);

	const uint32_t COLOR_GAMEAREA_BORDER = 0xFF3CCF54;

	// BALL OBJECTS

	void CreateBalls();

	const int MAX_BALLS = 2;
	std::vector<ObjectBall> m_ArrayBallObjects;

	// PAD OBJECTS

	inline int GetObjectPlayPadWidth(int windowHeight) { return windowHeight / 30; }
	inline int GetObjectPlayPadHeight(int windowHeight) { return windowHeight / 4; }
	inline double GetObjectPlayPadRadius(int windowHeight) { return 0.012962962962963 * windowHeight; }

	void CreatePads();

	ObjectPad m_ObjectPadLeft;
	ObjectPad m_ObjectPadRight;
};

#endif //__SCREENGAME_H__