#include "MiniginPCH.h"
#include <iostream>
#include "AudioLogger.h"

engine::AudioLogger::AudioLogger(Audio* audio, bool isMuted)
	:m_pAudioService{ audio }
	, m_IsMuted{ isMuted }
{
}

engine::AudioLogger::~AudioLogger()
{
	delete m_pAudioService;
}

void engine::AudioLogger::AddEffect(int soundID, const std::string& path)
{
	std::cout << "Added effect with ID: " + std::to_string(soundID) << "\n";
	if (!m_IsMuted)
		m_pAudioService->AddEffect(soundID, path);
}

void engine::AudioLogger::AddMusic(int soundID, const std::string& path)
{
	std::cout << "Added Music with ID: " + std::to_string(soundID) << "\n";
	if (!m_IsMuted)
		m_pAudioService->AddMusic(soundID, path);
}

void engine::AudioLogger::play(int soundID)
{
	std::cout << "Playing sound with ID: " + std::to_string(soundID) << "\n";
	if (!m_IsMuted)
		m_pAudioService->play(soundID);
}

void engine::AudioLogger::stopAll()
{
	std::cout << "Stopped all sounds\n";
	if(!m_IsMuted)
		m_pAudioService->stopAll();
}