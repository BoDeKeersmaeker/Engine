#include "MiniginPCH.h"
#include "AudioService.h"
#include <mutex>

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
	m_Infos.push_back(AudioInfo{ false, m_pMixChunks.size(), soundID });
	m_pMixChunks.push_back(Mix_LoadWAV(path.c_str()));
}

void engine::AudioService::AddMusic(int soundID, const std::string& path)
{
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
			AudioInfo temp = m_Infos[m_Queue.back()];
			m_Queue.pop();
			if (temp.isMusic) 
				Mix_PlayMusic(m_pMixMusic[temp.location], true);
			else
				Mix_PlayChannel(-1, m_pMixChunks[temp.location], 1);
		}

		tempLock.unlock();
	}
}

void engine::AudioService::play(int soundID)
{
	for(size_t i{ 0 } ; i < m_Infos.size(); i++)
		if (m_Infos[i].ID == soundID)
			m_Queue.push(i);
	
	m_ConditionVariable.notify_one();
}

void engine::AudioService::stopAll()
{
	Mix_HaltChannel(-1);
}