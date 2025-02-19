/**
 * SoundManager.h header file
 */

#pragma once

#ifndef __SOUNDMANAGER_H__
#define __SOUNDMANAGER_H__

#include <iostream>
#include <stdio.h>
#include <vector>

#include "miniaudio.h"

// Game classes (forward declaration)
class CZipManager;

class SoundManager
{
public:

	SoundManager(void);
	virtual ~SoundManager(void);

	void Initialize(CZipManager* resources);
	void Update(float deltaTime);

	// music playback

	void PlayMusicMenu(void);
	void PlayMusicGame(void);
	void ToggleMusic(void);

	// sound effect playback

	void PlaySoundGameStart(void);
	void PlaySoundGameOver();
	void PlaySoundToggle();
	void PlaySoundHitWall(void);
	void PlaySoundHitPad(void);

private:

	CZipManager* m_pZipManager;

	// music playback

	void ClearMusicResources(void);
	void PlayMusic(const char* pFilePath);
	void StopMusic(void);

	ma_device m_Device;
	ma_device_config m_DeviceConfig;
	ma_resource_manager m_ResourceManager;
	ma_resource_manager_config m_ResourceManagerConfig;
	ma_resource_manager_data_source m_DataSource;

	// sound effect playback

	void ClearSoundResources(void);
	void CreateSoundCache(std::vector<ma_sound*>* pArraySound, const char* pFilePath);
	void PlayCachedSound(std::vector<ma_sound*>* pArraySound, int* currentIndex, float soundVolume);

	void ResetFrameSound(void);
	void ActivateSoundDampen(void);
	void UpdateSoundDampen(float deltaTime);

	const int SOUND_CLIP_COUNT = 10;

	const float SOUND_DAMPEN_DURATION = 0.5f;
	const float SOUND_DAMPEN_INCREASE = 0.35f;

	std::vector<ma_sound*> m_ArraySoundHitWall;
	int m_iSoundHitWallIndex;

	std::vector<ma_sound*> m_ArraySoundHitPad;
	int m_iSoundHitPadIndex;

	ma_engine* m_pEngine;

	BOOL m_bInitialized;
	BOOL m_bIsMusicPlaying;
	BOOL m_bIsMusicPaused;

	BOOL m_bFrameSoundHitWall;
	BOOL m_bFrameSoundHitPad;

	BOOL m_bSoundDampenActive;
	float m_fSoundDampenTimer;
	float m_fSoundDampenCount;

	// FLAGS
	const BOOL ENABLE_AUDIO = TRUE;
	const BOOL ENABLE_MUSIC = TRUE;
};

#endif //__SOUNDMANAGER_H__