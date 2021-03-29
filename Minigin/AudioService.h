#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include "Audio.h"
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
		virtual ~AudioService();
		virtual void AddEffect(int soundID, const std::string& path);
		virtual void AddMusic(int soundID, const std::string& path);
		virtual void play(int soundID);
		virtual void stopAll();

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