/**
 * IApplication.h header file
 * 
 * An abstract class implements basic
 * features of game application, including
 * window handling etc.
 */

#pragma once

#ifndef __IAPPLICATION_H__
#define __IAPPLICATION_H__

#include <string>

 // Game classes
#include "Timer.h"

class IApplication
{
public:

	IApplication(void);
	virtual ~IApplication(void);

	/**
	 * Create
	 * initialise application main window with given parameters
	 * @param iWidth game window resolution width
	 * @param iHeight game window resolution height
	 */
	void Create();

	/**
	 * Release
	 * release application resources and shutdown
	 */
	virtual void Release(void);

	/**
	 * Close
	 * close application by sending the WM_CLOSE
	 * message to main window
	 */
	void Close(void);

	/**
	 * Run
	 * application message pump and main loop.
	 * Returns when application has closed.
	 * @return value from WM_QUIT message
	 */
	int Run(void);


	/**
	 * DoFlip
	 * function performs one screen update
	 */
	virtual void DoFlip(void);

	/**
	 * OnCreate
	 * called when application initialisation is ready
	 * This is a perfect place to initialise all game
	 * specific resources, textures, sounds, whatever...
	 * @return S_OK, or error code. If error code is returned
	 * application will shutdown.
	 */
	virtual HRESULT OnCreate(void) = 0;

	/**
	 * OnRelease
	 * called when application is about to shutdown
	 * This is a perfect place to free all game specific
	 * resources.
	 */
	virtual void OnRelease(void) = 0;

	/**
	 * OnFlip
	 * Application main loop, called just before graphics
	 * are transferred to screen.
	 */
	virtual void OnFlip(void) = 0;

	/**
	 * IsActive
	 * @return TRUE if application is active and updating
	 */
	inline BOOL IsActive() const { return m_bActive; }

	/**
	 * SetActive
	 * @param bSet TRUE to activate application, or FALSE
	 */
	void SetActive(BOOL bSet);

	/**
	 * GetTimer
	 * @return reference to application timer
	 */
	inline CTimer& GetTimer() { return m_Timer; }

	/**
	 * GetFrameTime
	 * @return number of seconds passed since last frame (frame delta)
	 */
	inline float GetFrameTime() const { return m_Timer.GetElapsedSeconds(); }

	/**
	 * RandInt
	 * @return random integer in between iLow and iHigh
	 */
	static int RandInt(int iLow, int iHigh);

	/**
	 * RandFloat
	 * @return random float in between fLow and fHigh
	 */
	static float RandFloat(float fLow, float fHigh);

	LPCWSTR StringToWideStr(std::string str);

	inline int GetWindowWidth() { return m_iWindowWidth; }
	inline int GetWindowHeight() { return m_iWindowHeight; }

protected:

	// app framerate
	const float FRAME_TIME = 160.0f;
	// window name
	const std::string WINDOW_NAME = "Pong Simulation";

	// Window handle
	HWND m_WindowHandle;

	// window size
	int m_iWindowWidth;
	int m_iWindowHeight;

	// full screen (yes/no)
	BOOL m_bFullScreen;

private:

	// application frame timer
	CTimer m_Timer;

	// do we exit the application
	BOOL m_bExitApp;
	// is application active
	BOOL m_bActive;
};

#endif	//__IAPPLICATION_H__