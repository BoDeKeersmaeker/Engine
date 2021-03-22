#include "MiniginPCH.h"
#include "AudioEffect.h"

engine::AudioEffect::AudioEffect(const std::string& path)
	:m_pMixChunk{ Mix_LoadWAV(path.c_str()) }
{
	
}

engine::AudioEffect::~AudioEffect()
{
	Mix_FreeChunk(m_pMixChunk);
	m_pMixChunk = nullptr;
}

void engine::AudioEffect::playSound(int soundID)
{
	if (m_pMixChunk != nullptr)
		Mix_PlayChannel(soundID, m_pMixChunk, 1);
}

void engine::AudioEffect::stopSound(int soundID)
{
	Mix_HaltChannel(soundID);
}

void engine::AudioEffect::stopAllSounds()
{
	Mix_HaltChannel(-1);
}