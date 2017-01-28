#include "Engine/Renderer/SpriteAnim.hpp"

SpriteAnim::SpriteAnim() 
: m_spriteSheet(nullptr)
, m_durationSeconds(0.f)
, m_animMode(ANIM_MODE_PLAY_TO_END)
, m_spriteStartIndex(0)
, m_spriteEndIndex(0)
, m_isPlaying(true)
, m_age(0.f)
, m_currIndex(0)
{ }
SpriteAnim::SpriteAnim(const SpriteSheet* spriteSheet, float durationSeconds, AnimMode spriteAnimMode, int spriteStartIndex, int spriteEndIndex)
    : m_spriteSheet(spriteSheet)
    , m_durationSeconds(durationSeconds)
    , m_animMode(spriteAnimMode)
    , m_spriteStartIndex(spriteStartIndex)
    , m_spriteEndIndex(spriteEndIndex)
	, m_isPlaying(true)
	, m_age(0.f)
	, m_currIndex(spriteStartIndex)
{ }

void SpriteAnim::Update(float deltaSeconds) {
	if (m_isPlaying)
		m_age += deltaSeconds;
}
AABB2 SpriteAnim::getCurrentTexCoords() const {
	int numIndexes = m_spriteEndIndex - m_spriteStartIndex + 1;

	float fractionThroughAnim = m_age / m_durationSeconds;
	int currSpriteIndex = (int)(fractionThroughAnim * numIndexes);

	return m_spriteSheet->GetTexCoordsForSpriteIndex(currSpriteIndex);
}
const Texture* SpriteAnim::getTexture() {
	return m_spriteSheet->GetTexture();
}
const SpriteSheet* SpriteAnim::getSpriteSheet() const {
	return m_spriteSheet;
}
void SpriteAnim::Pause() {
	m_isPlaying = false;
}
void SpriteAnim::Resume() {
	m_isPlaying = true;
}
void SpriteAnim::Reset() {
	m_age = 0.f;
}
bool SpriteAnim::isFinished() const {
	return (m_age >= m_durationSeconds);
}
bool SpriteAnim::isPlaying() const {
	return m_isPlaying;
}
float SpriteAnim::getDurationSeconds() const {
	return m_durationSeconds;
}
float SpriteAnim::getSecondsElapsed() const {
	return m_age;
}
float SpriteAnim::getSecondsRemaining() const {
	return m_durationSeconds - m_age;
}
float SpriteAnim::getFractionElapsed() const {
	return m_age / m_durationSeconds;
}
float SpriteAnim::getFractionRemaining() const {
	return (m_durationSeconds - m_age) / m_durationSeconds;
}
void SpriteAnim::setSecondsElapsed(float secondsElapsed) {
	m_age = secondsElapsed;
}
void SpriteAnim::setFractionElapsed(float fractionElapsed) {
	m_age = fractionElapsed * m_durationSeconds;
}