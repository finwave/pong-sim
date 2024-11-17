/**
 * ScreenMenu.h header file
 */

#pragma once

#ifndef __SCREENMENU_H__
#define __SCREENMENU_H__

#pragma warning(disable : 4995)

#include <iostream>

// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// Blend2d
#include <blend2d.h>

using namespace cv;
using namespace std;

// Game classes (forward declaration)
class CTheApp;

class ScreenMenu
{
public:

	ScreenMenu(void);
	virtual ~ScreenMenu(void);

	/**
	 * OnRelease
	 * Place to release all the assets of this game screen.
	 */
	void OnRelease(void);

	void Initialize();
	void Create(CTheApp* app);

	BOOL UpdateKeyboard(void);
	void UpdateScreen(float deltaTime);
	void RenderScreen(string windowName);

private:

	CTheApp* m_TheApp;

	void CreateFonts(void);

	void RenderTextTitle(BLContext* ctx);
	void RenderTextPlay(BLContext* ctx);
	void RenderTextOptions(BLContext* ctx);

	const double MARGIN_MULTIPLIER_VERTICAL = 0.2;

	// SCREEN TITLE

	const float FONT_SIZE_TITLE = 200.0f;
	const uint32_t COLOR_FONT_TITLE = 0xff4080ff;
	const char* m_StringTextTitle = "PONG SIMULATOR";

	BLFont m_FontTitle;
	BLGlyphBuffer m_GlyphBufferTitle;
	BLTextMetrics m_TextMetricsTitle;
	BLFontMetrics m_FontMetricsTitle;

	// PRESS TO PLAY

	const float FONT_SIZE_PLAY = 100.0f;
	const uint32_t COLOR_FONT_PLAY = 0xff3ccf54;
	const char* m_StringTextPlay = "PRESS ENTER TO START";

	BLFont m_FontPlay;
	BLGlyphBuffer m_GlyphBufferPlay;
	BLTextMetrics m_TextMetricsPlay;
	BLFontMetrics m_FontMetricsPlay;

	// OPTIONS

	const float FONT_SIZE_OPTIONS = 50.0f;
	const uint32_t COLOR_FONT_OPTIONS = 0xffc2c2c2;

	// OPTIONS / MUSIC

	const char* m_StringTextMusic = "Press \"M\" to toggle music on/off";

	BLFont m_FontMusic;
	BLGlyphBuffer m_GlyphBufferMusic;
	BLTextMetrics m_TextMetricsMusic;
	BLFontMetrics m_FontMetricsMusic;

	// OPTIONS / EXIT GAME

	const char* m_StringTextExit = "Press \"ESC\" to exit game";

	BLFont m_FontExit;
	BLGlyphBuffer m_GlyphBufferExit;
	BLTextMetrics m_TextMetricsExit;
	BLFontMetrics m_FontMetricsExit;
};

#endif //__SCREENMENU_H__