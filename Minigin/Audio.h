#pragma once

namespace engine
{
	class Audio
	{
	public:
		virtual ~Audio() = default;
		virtual void playSound(int soundID) = 0;
		virtual void stopSound(int soundID) = 0;
		virtual void stopAllSounds() = 0;
	};
}