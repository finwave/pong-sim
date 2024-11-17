/**
 * TheApp.h header file
 */

#pragma once

#ifndef __THEAPP_H__
#define __THEAPP_H__

#pragma warning(disable : 4995)

 // Game classes
#include "IApplication.h"
#include "SoundManager.h"
#include "ScreenMenu.h"
#include "ScreenGame.h"
#include "Resources.h"

using namespace std;

enum EState
{
	LoadingStart,
	LoadingResources,
	LoadingSound,
	LoadingGameScreens,
	StateMenu,
	StateGame
};

class CTheApp : public IApplication
{
public:

	CTheApp(void);
	virtual ~CTheApp(void);

	/**
	 * OnCreate
	 * called when application initialisation is ready
	 * This is a perfect place to initialise all game
	 * specific resources, textures, sounds, whatever...
	 * @return S_OK, or error code. If error code is returned
	 * application will shutdown.
	 */
	HRESULT OnCreate(void);

	/**
	 * OnRelease
	 * called when application is about to shutdown
	 * This is a perfect place to free all game specific
	 * resources.
	 */
	virtual void OnRelease(void);

	/**
	 * OnFlip
	 * Application main loop, called just before graphics
	 * are transferred to screen.
	 */
	void OnFlip(void);

	/**
	 * GetRandomSeed
	 * get random seed value
	 * @return unsigned integer value of random seed
	 */
	inline unsigned int GetRandomSeed() { return this->m_iRandomSeed; }

	inline EState GetCurrentState() { return this->m_State; }
	inline SoundManager* GetSoundManager() { return &m_SoundManager; }
	inline CResources* GetResourceManager() { return &m_Resources; }

	string IntToString(int iNumber);
	char* IntToChar(int iNumber);
	string IntToHex(int iNumber);

	wstring StringToWchar(const std::string& s);
	string WcharToString(const std::wstring& s);

private:

	BOOL GetWindowHandle(void);
	BOOL UpdateKeyboard(void);

	void UpdateScene(float);
	void RenderScene(void);
	void RenderLoading(string windowName);

	void SwitchState(EState);

	// resource files manager
	CResources m_Resources;

	// music and sSound effect manager
	SoundManager m_SoundManager;

	// game screens
	ScreenMenu m_ScreenMenu;
	ScreenGame m_ScreenGame;

	// game state
	EState m_State;

	// random seed
	unsigned int m_iRandomSeed;

	// render delta time calculations (based on 60fps)
	float m_fTargetDeltaTime;
	float m_fAccumulatedDeltaTime;

	// loading screen manual wait time (amount of frames)
	int m_iLoadingScreenWaitFrames = 30;
};

#endif	//__THEAPP_H__