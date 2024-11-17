/**
 * ScreenMenu.cpp source file
 */

 // Game classes
#include "TheApp.h"
#include "ScreenMenu.h"

ScreenMenu::ScreenMenu(void)
{
}

ScreenMenu::~ScreenMenu(void)
{
}

void ScreenMenu::OnRelease(void)
{
}

void ScreenMenu::Initialize()
{
	//TODO: USE APP TO GET WINDOW WIDTH AND HEIGHT
}

void ScreenMenu::Create(CTheApp* app)
{
	m_TheApp = app;
	CreateFonts();
}

void ScreenMenu::CreateFonts(void)
{
	// Load font-faces and handle possible errors.

	BLFontFace faceArcade;
	m_TheApp->GetResourceManager()->SetResourceFilePath("fonts/Arcade.ttf");
	BLResult result = faceArcade.createFromFile(m_TheApp->GetResourceManager()->GetResourceFilePath());

	if (result != BL_SUCCESS) {
		printf("Failed to load a font (err=%u)\n", result);
		return;
	}

	BLFontFace faceAtarianSystemRegular;
	m_TheApp->GetResourceManager()->SetResourceFilePath("fonts/SF Atarian System.ttf");
	result = faceAtarianSystemRegular.createFromFile(m_TheApp->GetResourceManager()->GetResourceFilePath());

	if (result != BL_SUCCESS) {
		printf("Failed to load a font (err=%u)\n", result);
		return;
	}

	m_FontTitle.createFromFace(faceArcade, FONT_SIZE_TITLE);
	m_FontPlay.createFromFace(faceArcade, FONT_SIZE_PLAY);
	m_FontMusic.createFromFace(faceAtarianSystemRegular, FONT_SIZE_OPTIONS);
	m_FontExit.createFromFace(faceAtarianSystemRegular, FONT_SIZE_OPTIONS);

	// Get text metrics.

	m_GlyphBufferTitle.setUtf8Text(m_StringTextTitle, SIZE_MAX);
	m_FontTitle.shape(m_GlyphBufferTitle);
	m_FontTitle.getTextMetrics(m_GlyphBufferTitle, m_TextMetricsTitle);
	m_FontMetricsTitle = m_FontTitle.metrics();

	m_GlyphBufferPlay.setUtf8Text(m_StringTextPlay, SIZE_MAX);
	m_FontPlay.shape(m_GlyphBufferPlay);
	m_FontPlay.getTextMetrics(m_GlyphBufferPlay, m_TextMetricsPlay);
	m_FontMetricsPlay = m_FontPlay.metrics();

	m_GlyphBufferMusic.setUtf8Text(m_StringTextMusic, SIZE_MAX);
	m_FontMusic.shape(m_GlyphBufferMusic);
	m_FontMusic.getTextMetrics(m_GlyphBufferMusic, m_TextMetricsMusic);
	m_FontMetricsMusic = m_FontMusic.metrics();

	m_GlyphBufferExit.setUtf8Text(m_StringTextExit, SIZE_MAX);
	m_FontExit.shape(m_GlyphBufferExit);
	m_FontExit.getTextMetrics(m_GlyphBufferExit, m_TextMetricsExit);
	m_FontMetricsExit = m_FontExit.metrics();
}

BOOL ScreenMenu::UpdateKeyboard(void)
{
	// 0x01 = single key press
	// 0x8000 = continuous

	return TRUE;
}

void ScreenMenu::UpdateScreen(float deltaTime)
{
}

void ScreenMenu::RenderScreen(string windowName)
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

	RenderTextTitle(&ctx);
	RenderTextPlay(&ctx);
	RenderTextOptions(&ctx);

	// Finish Blend2d render commands.
	ctx.end();

	BLImageData imgData;
	img.getData(&imgData);

	// Show our image inside the window.
	Mat cvImg(img.height(), img.width(), CV_8UC4, imgData.pixelData);
	imshow(windowName, cvImg);
}

void ScreenMenu::RenderTextTitle(BLContext* ctx)
{
	BLRgba32 color(COLOR_FONT_TITLE);

	int windowWidth = m_TheApp->GetWindowWidth();
	int windowHeight = m_TheApp->GetWindowHeight();

	double posX = (double)windowWidth / 2.0 - m_TextMetricsTitle.boundingBox.x1 / 2.0;

	// SETS VERTICAL POSITION TO TOP
	//double posY = (m_FontMetricsTitle.ascent - m_FontMetricsTitle.descent / 2.0) / 2.0;
	//posY += m_FontMetricsTitle.descent / 2.0;
	
	//posY += (double)windowHeight / 4.0;

	double posY = (m_FontMetricsTitle.ascent - m_FontMetricsTitle.descent / 2.0);
	posY += MARGIN_MULTIPLIER_VERTICAL * (double)windowHeight;

	BLPoint position(posX, posY);
	ctx->fillGlyphRun(position, m_FontTitle, m_GlyphBufferTitle.glyphRun(), color);
}

void ScreenMenu::RenderTextPlay(BLContext* ctx)
{
	BLRgba32 color(COLOR_FONT_PLAY);

	int windowWidth = m_TheApp->GetWindowWidth();
	int windowHeight = m_TheApp->GetWindowHeight();

	double posX = (double)windowWidth / 2.0 - m_TextMetricsPlay.boundingBox.x1 / 2.0;

	double posY = (m_FontMetricsPlay.ascent - m_FontMetricsPlay.descent / 2.0) / 2.0;
	posY += m_FontMetricsPlay.descent / 2.0;
	posY += (double)windowHeight / 2.0;

	BLPoint position(posX, posY);
	ctx->fillGlyphRun(position, m_FontPlay, m_GlyphBufferPlay.glyphRun(), color);
}

void ScreenMenu::RenderTextOptions(BLContext* ctx)
{
	BLRgba32 color(COLOR_FONT_OPTIONS);

	int windowWidth = m_TheApp->GetWindowWidth();
	int windowHeight = m_TheApp->GetWindowHeight();

	double posX = (double)windowWidth / 2.0 - m_TextMetricsMusic.boundingBox.x1 / 2.0;

	/*
	double posY = (m_FontMetricsMusic.ascent - m_FontMetricsMusic.descent / 2.0) / 2.0;
	posY += m_FontMetricsMusic.descent / 2.0;
	posY += (double)windowHeight / 2.0;
	*/

	double posY = (double)windowHeight - m_FontMetricsMusic.descent;
	posY -= MARGIN_MULTIPLIER_VERTICAL * (double)windowHeight;

	BLPoint position(posX, posY);
	ctx->fillGlyphRun(position, m_FontMusic, m_GlyphBufferMusic.glyphRun(), color);

	float textHeight = m_FontMetricsMusic.ascent - m_FontMetricsMusic.descent;
	position.x = (double)windowWidth / 2.0 - m_TextMetricsExit.boundingBox.x1 / 2.0;
	position.y += 1.75f * textHeight;

	ctx->fillGlyphRun(position, m_FontExit, m_GlyphBufferExit.glyphRun(), color);
}
