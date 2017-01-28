#pragma once

#include "Game/BlockDefinition.hpp"
#include "Engine/Audio.hpp"
#include "Engine/Math/MathUtils.hpp"

class AudioPlayer;
extern AudioPlayer* g_theAudioPlayer;

const float FOOTSTEPS_VARIANCE = 0.2f;
const float FOOTSTEPS_SPEED = 5.f;

class AudioPlayer {
public:
	//STRUCTORS
	AudioPlayer();
	~AudioPlayer();

	//UPDATE
	void Update(float deltaSeconds);

	//PLAY SOUNDS
	void PlayFootsteps(float distanceTraveled, const vector<BlockType>& blockTypes);
	void PlayPlaceBlock(BlockType type);
	void PlayBreakBlock(BlockType type);
	void PlaySwitchBlock();

	void PlayMusic(float volumeLevel = 1.f);
	void PlayRainSound();
	void StopRain();

	void ChangeVolumeOfRainSound(float volumeLevel);

private:
	//Data members
	float m_currFootstepDistance;
	float m_distSinceLastPlayedFootstep;

	AudioSystem m_audioSystem;
	SoundID m_switchBlockID;

	//Private member functions
	void PlayFootstep(const SoundID& soundToPlay, float distanceTraveledThisFrame);
	void CalculateNewFootstepDistance();

	AudioChannelHandle m_musicChannel;
	AudioChannelHandle m_rainChannel;

	const char* m_musicPath = "Data/Sounds/Music.mp3";
	const char* m_rainPath = "Data/Sounds/Rain.mp3";

	SoundID m_musicID;
	SoundID m_rainID;

	bool m_isRainPlaying;
};