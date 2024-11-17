/**
 * SoundManager.cpp source file
 */

#define MINIAUDIO_IMPLEMENTATION

// Game classes
#include "SoundManager.h"
#include "Resources.h"

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
	ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
	if (pDecoder == NULL) {
		return;
	}

	/* Reading PCM frames will loop based on what we specified when called ma_data_source_set_looping(). */
	ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

	(void)pInput;
}

SoundManager::SoundManager(void)
{
	// initialise member variables
	m_bInitialized = FALSE;
	m_bIsMusicPlaying = FALSE;
}

SoundManager::~SoundManager(void)
{
	if (m_bInitialized)
	{
		ClearMusicResources();
		ClearSoundResources();
	}
}

void SoundManager::Initialize(CResources* resources)
{
	m_Resources = resources;
	m_bInitialized = FALSE;

	if (!ENABLE_AUDIO)
	{
		return;
	}

	// MUSIC

	// Create device configuration.
	m_DeviceConfig = ma_device_config_init(ma_device_type_playback);
	m_DeviceConfig.dataCallback = data_callback;
	m_DeviceConfig.pUserData = &m_DataSource;    /* <-- We'll be reading from this in the data callback. */

	// Initialize device manager.
	m_ResourceManagerConfig = ma_resource_manager_config_init();

	// SOUND EFFECT

	m_pEngine = (ma_engine*)malloc(sizeof(ma_engine));

	if (ma_engine_init(NULL, m_pEngine) != MA_SUCCESS) {
		return;
	}

	m_Resources->SetResourceFilePath("sound/hit_wall.wav");
	CreateSoundCache(&m_ArraySoundHitWall, m_Resources->GetResourceFilePath());

	m_Resources->SetResourceFilePath("sound/hit_pad.wav");
	CreateSoundCache(&m_ArraySoundHitPad, m_Resources->GetResourceFilePath());

	m_bInitialized = TRUE;
}

void SoundManager::Update(float deltaTime)
{
	ResetFrameSound();
	UpdateSoundDampen(deltaTime);
}

#pragma region MUSIC

void SoundManager::ClearMusicResources(void)
{
	if (!ENABLE_MUSIC)
	{
		return;
	}

	if (!m_bInitialized)
	{
		return;
	}

	/* Uninitialize the device first to ensure the data callback is stopped and doesn't try to access any data. */
	ma_device_uninit(&m_Device);

	/*
	Before uninitializing the resource manager we need to uninitialize every data source. The data source is owned by
	the caller which means you're responsible for uninitializing it.
	*/
	ma_resource_manager_data_source_uninit(&m_DataSource);

	/* Uninitialize the resource manager after each data source. */
	ma_resource_manager_uninit(&m_ResourceManager);
}

void SoundManager::PlayMusicMenu(void)
{
	m_Resources->SetResourceFilePath("music/menu.mp3");
	PlayMusic(m_Resources->GetResourceFilePath());
}

void SoundManager::PlayMusicGame(void)
{
	m_Resources->SetResourceFilePath("music/game.mp3");
	PlayMusic(m_Resources->GetResourceFilePath());
}

void SoundManager::PlayMusic(const char* pFilePath)
{
	if (!ENABLE_MUSIC)
	{
		return;
	}

	StopMusic();

	// Initialize device.
	if (ma_device_init(NULL, &m_DeviceConfig, &m_Device) != MA_SUCCESS)
	{
		printf("Failed to initialize playback device.\n");
		return;
	}

	// Initialize device manager.
	m_ResourceManagerConfig.decodedFormat = m_Device.playback.format;
	m_ResourceManagerConfig.decodedChannels = m_Device.playback.channels;
	m_ResourceManagerConfig.decodedSampleRate = m_Device.sampleRate;

	if (ma_resource_manager_init(&m_ResourceManagerConfig, &m_ResourceManager) != MA_SUCCESS)
	{
		printf("Failed to initialize resource manager.");
		ma_device_uninit(&m_Device);
		return;
	}

	// Now that we have a resource manager we can load a sound.
	if (ma_resource_manager_data_source_init(
		&m_ResourceManager, pFilePath,
		MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_DECODE | MA_RESOURCE_MANAGER_DATA_SOURCE_FLAG_STREAM,
		NULL,   /* Async notification. */
		&m_DataSource) != MA_SUCCESS)
	{
		printf("Failed to load sound.");
		ClearMusicResources();
	}

	// Enable looping.
	ma_data_source_set_looping(&m_DataSource, MA_TRUE);

	/* Now that we have a sound we can start the device. */
	if (!m_bIsMusicPaused)
	{
		if (ma_device_start(&m_Device) != MA_SUCCESS)
		{
			printf("Failed to start 'playback device.\n");
			ma_device_uninit(&m_Device);
			return;
		}
	}

	m_bIsMusicPlaying = TRUE;
}

void SoundManager::StopMusic(void)
{
	if (!m_bInitialized)
	{
		return;
	}

	if (m_bIsMusicPlaying)
	{
		ClearMusicResources();
	}

	m_bIsMusicPlaying = FALSE;
}

void SoundManager::ToggleMusic(void)
{
	if (m_bIsMusicPlaying)
	{
		if (m_bIsMusicPaused)
		{
			ma_device_start(&m_Device);
		}
		else
		{
			ma_device_stop(&m_Device);
		}

		m_bIsMusicPaused = !m_bIsMusicPaused;
	}
}

#pragma endregion

#pragma region SOUND EFFECT

void SoundManager::ClearSoundResources(void)
{
	if (!m_bInitialized)
	{
		return;
	}

	if (!m_ArraySoundHitWall.empty())
	{
		for (std::vector<ma_sound*>::iterator it = m_ArraySoundHitWall.begin(); it != m_ArraySoundHitWall.end(); ++it)
		{
			ma_sound* pSound = *it;
			ma_sound_uninit(pSound);
		}

		m_ArraySoundHitWall.clear();
	}

	if (!m_ArraySoundHitPad.empty())
	{
		for (std::vector<ma_sound*>::iterator it = m_ArraySoundHitPad.begin(); it != m_ArraySoundHitPad.end(); ++it)
		{
			ma_sound* pSound = *it;
			ma_sound_uninit(pSound);
		}

		m_ArraySoundHitPad.clear();
	}

	ma_engine_uninit(m_pEngine);
}

void SoundManager::CreateSoundCache(std::vector<ma_sound*>* pArraySound, const char* pFilePath)
{
	pArraySound->reserve(SOUND_CLIP_COUNT);

	for (int i = 0; i < SOUND_CLIP_COUNT; i++)
	{
		ma_sound* sound = (ma_sound*)malloc(sizeof(ma_sound));
		ma_result result = ma_sound_init_from_file(m_pEngine, pFilePath, 0, NULL, NULL, sound);

		if (result == MA_SUCCESS)
		{
			pArraySound->push_back(sound);
		}
	}
}

void SoundManager::PlayCachedSound(std::vector<ma_sound*>* pArraySound, int* targetIndex, float soundVolume)
{
	if (!pArraySound->empty())
	{
		int currentIndex = *targetIndex;
		int iterateIndex = -1;

		for (std::vector<ma_sound*>::iterator it = pArraySound->begin(); it != pArraySound->end(); ++it)
		{
			iterateIndex++;

			if (iterateIndex == currentIndex)
			{
				ma_sound* pSound = *it;

				ma_sound_seek_to_pcm_frame(pSound, 0);
				ma_sound_set_volume(pSound, soundVolume);
				ma_sound_start(pSound);

				int nextIndex = currentIndex + 1;

				if (nextIndex >= SOUND_CLIP_COUNT)
				{
					nextIndex = 0;
				}

				*targetIndex = nextIndex;
			}
		}
	}
}

void SoundManager::PlaySoundGameStart(void)
{
	if (!m_bInitialized)
	{
		return;
	}

	m_Resources->SetResourceFilePath("sound/game_start.wav");
	ma_engine_play_sound(m_pEngine, m_Resources->GetResourceFilePath(), NULL);
}

void SoundManager::PlaySoundGameOver(void)
{
	if (!m_bInitialized)
	{
		return;
	}
}

void SoundManager::PlaySoundToggle(void)
{
	if (!m_bInitialized)
	{
		return;
	}

	m_Resources->SetResourceFilePath("sound/button_toggle.wav");
	ma_engine_play_sound(m_pEngine, m_Resources->GetResourceFilePath(), NULL);
}

void SoundManager::PlaySoundHitWall(void)
{
	if (!m_bInitialized || !m_bFrameSoundHitWall)
	{
		return;
	}

	ActivateSoundDampen();
	float soundVolume = 1.0f / m_fSoundDampenCount;

	PlayCachedSound(&m_ArraySoundHitWall, &m_iSoundHitWallIndex, soundVolume);
	m_bFrameSoundHitWall = FALSE;
}

void SoundManager::PlaySoundHitPad(void)
{
	if (!m_bInitialized || !m_bFrameSoundHitPad)
	{
		return;
	}

	ActivateSoundDampen();
	float soundVolume = 1.0f / m_fSoundDampenCount;

	PlayCachedSound(&m_ArraySoundHitPad, &m_iSoundHitPadIndex, soundVolume);
	m_bFrameSoundHitPad = FALSE;
}

void SoundManager::ResetFrameSound(void)
{
	m_bFrameSoundHitWall = TRUE;
	m_bFrameSoundHitPad = TRUE;
}

void SoundManager::ActivateSoundDampen(void)
{
	if (m_bSoundDampenActive)
	{
		m_fSoundDampenCount += SOUND_DAMPEN_INCREASE;
		return;
	}

	m_bSoundDampenActive = TRUE;
	m_fSoundDampenTimer = SOUND_DAMPEN_DURATION;
	m_fSoundDampenCount = 1;
}

void SoundManager::UpdateSoundDampen(float deltaTime)
{
	if (!m_bSoundDampenActive)
	{
		return;
	}

	m_fSoundDampenTimer -= deltaTime;

	if (m_fSoundDampenTimer <= 0)
	{
		m_bSoundDampenActive = FALSE;
	}
}

#pragma endregion