#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include "Audio.h"
#include <SDL_mixer.h>

namespace engine
{
	struct AudioInfo
	{
		bool isMusic;
		size_t location;
		int ID;
	};
	
	class AudioService final : public Audio
	{
	public:
		AudioService();
		~AudioService() override;

		AudioService(const AudioService& other) = delete;
		AudioService(AudioService&& other) = delete;
		AudioService& operator=(const AudioService& other) = delete;
		AudioService& operator=(AudioService&& other) = delete;
		
		void AddEffect(int soundID, const std::string& path) override;
		void AddMusic(int soundID, const std::string& path) override;
		void play(int soundID) override;
		void stopAll() override;
		void SetVolume(int volume) override;
		void Reset() override;
	
	private:
		void Update();
		
		std::vector<Mix_Chunk*> m_pMixChunks;
		std::vector<Mix_Music*> m_pMixMusic;
		std::vector<AudioInfo> m_Infos;
		std::queue<size_t> m_Queue;
		std::thread m_Thread;
		std::mutex m_Mutex;
		std::condition_variable m_ConditionVariable;
		bool m_ServiceShutdown = false;
	};
}