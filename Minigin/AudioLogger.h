#pragma once
#include "Audio.h"

namespace engine
{
	class AudioLogger : public Audio
	{
	public:
		AudioLogger(Audio* audio );
		virtual ~AudioLogger();
		virtual void playSound(int soundID);
		virtual void stopSound(int soundID);
		virtual void stopAllSounds();

	private:
		Audio* m_pAudioEffect;
	};
}