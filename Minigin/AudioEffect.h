#pragma once
#include "Audio.h"

namespace engine
{
	class AudioEffect : public Audio
	{
	public:
		AudioEffect(const std::string& path);
		virtual ~AudioEffect();
		virtual void playSound(int soundID);
		virtual void stopSound(int soundID);
		virtual void stopAllSounds();

	private:
		Mix_Chunk* m_pMixChunk;
	};
}