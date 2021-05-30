#pragma once
#include <condition_variable>
#include <map>
#include <queue>
#include <thread>
#include "Singleton.h"

constexpr auto NODE_DEBUG = 1;
constexpr auto GRID_DEBUG = 2;


namespace engine
{
	class DebugManager final : public Singleton<DebugManager>
	{
	public:
		virtual ~DebugManager() override;
		
		//Use -1 to set all channels
		void SetChannelActivated(bool isEnabled, size_t debugChannel = -1);
		void print(std::string text, size_t debugChannel = 0);
	
	private:
		friend class Singleton<DebugManager>;
		DebugManager();

		void Update();

		std::map<size_t, bool> m_Channels;
		std::queue<std::string> m_Queue;
		std::thread m_Thread;
		std::mutex m_Mutex;
		std::condition_variable m_ConditionVariable;
		bool m_ServiceShutdown = false;
	};
}