#include "MiniginPCH.h"
#include "AudioService.h"
#include <mutex>
#include "DebugManager.h"

engine::AudioService::AudioService()
	: m_Thread{ &engine::AudioService::Update, this }
{
	
}

engine::AudioService::~AudioService()
{
	for (Mix_Chunk* chunk : m_pMixChunks)
		Mix_FreeChunk(chunk);

	for (Mix_Music* music : m_pMixMusic)
		Mix_FreeMusic(music);

	m_ServiceShutdown = true;
	m_ConditionVariable.notify_one();
	m_Thread.join();
}

void engine::AudioService::AddEffect(int soundID, const std::string& path)
{
	DebugManager::GetInstance().print("Added effect with ID: " + std::to_string(soundID), AUDIO_DEBUG);
	m_Infos.push_back(AudioInfo{ false, m_pMixChunks.size(), soundID });
	m_pMixChunks.push_back(Mix_LoadWAV(path.c_str()));
}

void engine::AudioService::AddMusic(int soundID, const std::string& path)
{
	DebugManager::GetInstance().print("Added Music with ID: " + std::to_string(soundID), AUDIO_DEBUG);
	m_Infos.push_back(AudioInfo{ true, m_pMixMusic.size(), soundID });
	m_pMixMusic.push_back(Mix_LoadMUS(path.c_str()));
}

void engine::AudioService::Update()
{
	while (!m_ServiceShutdown)
	{
		std::unique_lock<std::mutex> tempLock{ m_Mutex };
		m_ConditionVariable.wait(tempLock, [this]() {return (!m_Queue.empty() || m_ServiceShutdown); });

		if(!m_Queue.empty())
		{
			const AudioInfo temp = m_Infos[m_Queue.front()];
			m_Queue.pop();
			if (temp.isMusic) 
				Mix_PlayMusic(m_pMixMusic[temp.location], 1);
			else
				Mix_PlayChannel(-1, m_pMixChunks[temp.location], 0);
		}

		tempLock.unlock();
	}
}

void engine::AudioService::play(int soundID)
{
	DebugManager::GetInstance().print("Playing sound with ID: " + std::to_string(soundID), AUDIO_DEBUG);
	for(size_t i{ 0 } ; i < m_Infos.size(); i++)
		if (m_Infos[i].ID == soundID)
			m_Queue.push(i);
	
	m_ConditionVariable.notify_one();
}

void engine::AudioService::stopAll()
{
	DebugManager::GetInstance().print("Stopped all sounds", AUDIO_DEBUG);
	Mix_HaltChannel(-1);
}

void engine::AudioService::SetVolume(int volume)
{
	DebugManager::GetInstance().print("Set volume to " + std::to_string(volume), AUDIO_DEBUG);
	Mix_VolumeMusic(volume);
	Mix_VolumeMusic(volume);
}

void engine::AudioService::Reset()
{
	DebugManager::GetInstance().print("Resetting the audio service", AUDIO_DEBUG);
	m_pMixChunks.clear();
	m_pMixMusic.clear();
	m_Infos.clear();
	while (!m_Queue.empty())
		m_Queue.pop();
}