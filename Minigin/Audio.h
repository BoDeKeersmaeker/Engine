#pragma once
#include <string>

namespace engine
{
	class Audio
	{
	public:
		Audio() = default;
		virtual ~Audio() = default;
		
		Audio(const Audio& other) = delete;
		Audio(Audio&& other) = delete;
		Audio& operator=(const Audio& other) = delete;
		Audio& operator=(Audio&& other) = delete;
		
		virtual void AddEffect(int soundID, const std::string& path) = 0;
		virtual void AddMusic(int soundID, const std::string& path) = 0;
		virtual void play(int soundID) = 0;
		virtual void stopAll() = 0;
		virtual void SetVolume(int volume) = 0;
		virtual void Reset() = 0;
	};
}