#pragma once
#include "Audio.h"

namespace engine
{
	class AudioLogger : public Audio
	{
	public:
		AudioLogger(Audio* audio, bool isMuted = false);
		virtual ~AudioLogger();
		virtual void AddEffect(int soundID, const std::string& path) override;
		virtual void AddMusic(int soundID, const std::string& path) override;
		virtual void play(int soundID) override;
		virtual void stopAll() override;
		virtual void SetVolume(int volume) override;
		virtual void Reset() override;
	
	private:
		Audio* m_pAudioService;
		bool m_IsMuted;
	};
}