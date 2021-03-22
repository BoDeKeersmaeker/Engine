#pragma once

namespace engine
{
	class Audio;
	class NullAudio;
	
	class AudioLocator
	{
	public:
		static Audio* getAudioSystem();
		static void registerAudio(Audio* audio);

	private:
		AudioLocator() = default;

		static NullAudio m_DefaultAudio;
		static Audio* m_AudioInstance;
	};
}