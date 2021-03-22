#pragma once
#include "Audio.h"
namespace engine
{
	class NullAudio : public Audio
	{
	public:
		NullAudio() = default;
		virtual ~NullAudio() = default;
		virtual void playSound(int) {};
		virtual void stopSound(int) {};
		virtual void stopAllSounds() {};
	};
}