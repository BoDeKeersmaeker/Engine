#pragma once

namespace engine
{
	class Audio
	{
	public:
		virtual ~Audio() = default;
		virtual void AddEffect(int soundID, const std::string& path) = 0;
		virtual void AddMusic(int soundID, const std::string& path) = 0;
		virtual void play(int soundID) = 0;
		virtual void stopAll() = 0;
	};
}