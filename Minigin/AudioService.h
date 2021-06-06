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
	
	class AudioService : public Audio
	{
	public:
		AudioService();
		virtual ~AudioService() override;
		virtual void AddEffect(int soundID, const std::string& path) override;
		virtual void AddMusic(int soundID, const std::string& path) override;
		virtual void play(int soundID) override;
		virtual void stopAll() override;
		virtual void SetVolume(int volume) override;
		virtual void Reset() override;
	
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