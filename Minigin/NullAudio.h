#pragma once
#include "Audio.h"
namespace engine
{
	class NullAudio final : public Audio
	{
	public:
		NullAudio() = default;
		~NullAudio() override = default;

		NullAudio(const NullAudio& other) = delete;
		NullAudio(NullAudio&& other) = delete;
		NullAudio& operator=(const NullAudio& other) = delete;
		NullAudio& operator=(NullAudio&& other) = delete;
		
		void AddEffect(int, const std::string&) override {};
		virtual void AddMusic(int, const std::string&) override {};
		virtual void play(int) override {};
		virtual void stopAll() override {};
		virtual void SetVolume(int) override {};
		virtual void Reset() override{};
	};
}