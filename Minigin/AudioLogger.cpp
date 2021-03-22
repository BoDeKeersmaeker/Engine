#include "MiniginPCH.h"
#include "AudioLogger.h"

engine::AudioLogger::AudioLogger(Audio* audio)
	:m_pAudioEffect{ audio }
{
}

engine::AudioLogger::~AudioLogger()
{
	delete m_pAudioEffect;
}

void engine::AudioLogger::playSound(int soundID)
{
	std::cout << "Playing sound: " + std::to_string(soundID) << "\n";
	m_pAudioEffect->playSound(soundID);
}

void engine::AudioLogger::stopSound(int soundID)
{
	std::cout << "Stopped sound: " + std::to_string(soundID) << "\n";
	m_pAudioEffect->stopSound(soundID);

}

void engine::AudioLogger::stopAllSounds()
{
	std::cout << "Stopped all sounds \n";
	m_pAudioEffect->stopAllSounds();
}
