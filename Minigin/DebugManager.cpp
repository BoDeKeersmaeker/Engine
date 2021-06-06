#include "MiniginPCH.h"
#include <iostream>
#include "DebugManager.h"

engine::DebugManager::DebugManager()
	: Singleton()
	, m_Thread{ &engine::DebugManager::Update, this }
{
	
}

engine::DebugManager::~DebugManager()
{
	m_ServiceShutdown = true;
	m_ConditionVariable.notify_one();
	m_Thread.join();
}

void engine::DebugManager::SetChannelDeactivated(bool isDisabled, size_t debugChannel)
{
	if (debugChannel == -1)
	{
		if (m_Channels.size() > 0)
		{
			for (size_t i{ 0 }; i < m_Channels.size(); i++)
				m_Channels[i] = isDisabled;
		}
		else
		{
			for (size_t i{ 0 }; i < DEBUG_CHANNEL_SIZE; i++)
				m_Channels[i] = isDisabled;
		}
	}
	else
		m_Channels[debugChannel] = isDisabled;
}

void engine::DebugManager::print(std::string text, size_t debugChannel)
{
	if(m_Channels[debugChannel])
		return;
	
	m_Queue.push(text);
	m_ConditionVariable.notify_one();
}

void engine::DebugManager::Update()
{
	while (!m_ServiceShutdown)
	{
		std::unique_lock<std::mutex> tempLock{ m_Mutex };
		m_ConditionVariable.wait(tempLock, [this]() {return (!m_Queue.empty() || m_ServiceShutdown); });

		if (!m_Queue.empty())
		{
			std::cout << m_Queue.front() << std::endl;
			m_Queue.pop();
		}
		
		tempLock.unlock();
	}
}