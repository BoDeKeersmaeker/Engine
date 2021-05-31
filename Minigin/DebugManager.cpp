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

void engine::DebugManager::SetChannelActivated(bool isEnabled, size_t debugChannel)
{
	if (debugChannel == -1)
		for (size_t i{ 0 }; i <= m_Channels.size(); i++)
			m_Channels[i] = isEnabled;
	else
		m_Channels[debugChannel] = isEnabled;
}

void engine::DebugManager::print(std::string text, size_t debugChannel)
{
	if(!m_Channels[debugChannel])
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
