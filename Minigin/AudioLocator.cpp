#include "MiniginPCH.h"
#include "AudioLocator.h"
#include "Audio.h"
#include "NullAudio.h"

engine::NullAudio engine::AudioLocator::m_DefaultAudio{};
engine::Audio* engine::AudioLocator::m_AudioInstance = &m_DefaultAudio;

engine::Audio* engine::AudioLocator::getAudioSystem()
{
	return m_AudioInstance;
}

void engine::AudioLocator::registerAudio(Audio* audio)
{
	m_AudioInstance = audio;
}