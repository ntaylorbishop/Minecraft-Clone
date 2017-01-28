#include "Game/AudioPlayer.hpp"

AudioPlayer* g_theAudioPlayer = nullptr;

//STRUCTORS------------------------------------------------------------------------------------------------------------------------------------------------
AudioPlayer::AudioPlayer() 
    : m_distSinceLastPlayedFootstep(0.f)
	, m_currFootstepDistance(0.f)
	, m_isRainPlaying(false)
	, m_musicID(0)
	, m_rainID(0)
{ 
	m_switchBlockID = m_audioSystem.CreateOrGetSound("Data/Sounds/SwitchBlock.wav");
	m_musicID = m_audioSystem.CreateOrGetSound(m_musicPath);
	m_rainID = m_audioSystem.CreateOrGetSound(m_rainPath);

	CalculateNewFootstepDistance();
}
AudioPlayer::~AudioPlayer() { }

//UPDATE------------------------------------------------------------------------------------------------------------------------------------------------
void AudioPlayer::Update(float deltaSeconds) {
	m_audioSystem.Update(deltaSeconds);
}

//PLAY SOUNDS-----------------------------------------------------------------------------------------------------------------------------------------
void AudioPlayer::PlayFootsteps(float distanceTraveled, const vector<BlockType>& blockTypes) {
	vector<string> footstepSounds;

	for (unsigned int i = 0; i < blockTypes.size(); i++) {
		vector<string> currFootstepSounds = BlockDefinition::GetBlockFootstepSounds(blockTypes[i]);

		footstepSounds.insert(footstepSounds.end(), currFootstepSounds.begin(), currFootstepSounds.end());
	}

	if (!footstepSounds.empty()) {
		int whichSoundToPlay = rand() % footstepSounds.size();

		SoundID footstepSoundToPlay = m_audioSystem.CreateOrGetSound(footstepSounds[whichSoundToPlay]);
		PlayFootstep(footstepSoundToPlay, distanceTraveled);
	}
}
void AudioPlayer::PlayPlaceBlock(BlockType type) {
	vector<string> placeBlockSoundPaths = BlockDefinition::GetBlockPlaceSounds(type);
	vector<SoundID> placeBlockSoundIDs;

	for (unsigned int i = 0; i < placeBlockSoundPaths.size(); i++) {
		placeBlockSoundIDs.push_back(m_audioSystem.CreateOrGetSound(placeBlockSoundPaths[i]));
	}

	int whichSoundToPlay = rand() % placeBlockSoundIDs.size();

	m_audioSystem.Play(placeBlockSoundIDs[whichSoundToPlay]);
}
void AudioPlayer::PlayBreakBlock(BlockType type) {
	vector<string> digBlockSoundPaths = BlockDefinition::GetBlockDigSounds(type);
	vector<SoundID> digBlockSoundIDs;

	for (unsigned int i = 0; i < digBlockSoundPaths.size(); i++) {
		digBlockSoundIDs.push_back(m_audioSystem.CreateOrGetSound(digBlockSoundPaths[i]));
	}

	int whichSoundToPlay = rand() % digBlockSoundIDs.size();

	m_audioSystem.Play(digBlockSoundIDs[whichSoundToPlay]);
}
void AudioPlayer::PlaySwitchBlock() {
	m_audioSystem.Play(m_switchBlockID);
}
void AudioPlayer::PlayMusic(float volumeLevel) {
	m_musicChannel = m_audioSystem.PlayLoop(m_musicID, volumeLevel);
}

//PRIVATE MEMBER FUNCTIONS--------------------------------------------------------------------------------------------------------------------------------
void AudioPlayer::PlayFootstep(const SoundID& soundToPlay, float distanceTraveledThisFrame) {
	m_distSinceLastPlayedFootstep += distanceTraveledThisFrame;

	if (m_distSinceLastPlayedFootstep > m_currFootstepDistance) {
		m_audioSystem.Play(soundToPlay, 0.5f);
		CalculateNewFootstepDistance();
	}
}
void AudioPlayer::CalculateNewFootstepDistance() {
	float varianceOnFootstep = Randf(-FOOTSTEPS_VARIANCE, FOOTSTEPS_VARIANCE);

	m_currFootstepDistance = FOOTSTEPS_SPEED + varianceOnFootstep;
	m_distSinceLastPlayedFootstep = 0.f;
}

void AudioPlayer::PlayRainSound() {
	if (!m_isRainPlaying) {
		m_rainChannel = m_audioSystem.PlayLoop(m_rainID);
		m_isRainPlaying = true;
	}
}
void AudioPlayer::StopRain() {
	m_audioSystem.StopChannel(m_rainChannel);
	m_isRainPlaying = false;
}

void AudioPlayer::ChangeVolumeOfRainSound(float volumeLevel) {
	m_audioSystem.ChangeVolumeOfChannel(m_rainChannel, volumeLevel);
}