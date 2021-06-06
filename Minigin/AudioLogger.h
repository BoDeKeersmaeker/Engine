#pragma once
#include "Audio.h"

namespace engine
{
	class AudioLogger final : public Audio
	{
	public:
		AudioLogger(Audio* audio, bool isMuted = false);
		~AudioLogger() override;
		
		AudioLogger(const AudioLogger& other) = delete;
		AudioLogger(AudioLogger&& other) = delete;
		AudioLogger& operator=(const AudioLogger& other) = delete;
		AudioLogger& operator=(AudioLogger&& other) = delete;
		
		void AddEffect(int soundID, const std::string& path) override;
		void AddMusic(int soundID, const std::string& path) override;
		void play(int soundID) override;
		void stopAll() override;
		void SetVolume(int volume) override;
		void Reset() override;
	
	private:
		Audio* m_pAudioService;
		bool m_IsMuted;
	};
}