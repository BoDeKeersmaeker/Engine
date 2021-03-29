#pragma once
#include "Audio.h"
namespace engine
{
	class NullAudio : public Audio
	{
	public:
		NullAudio() = default;
		virtual ~NullAudio() = default;
		virtual void AddEffect(int, const std::string&) override {};
		virtual void AddMusic(int, const std::string&) override {};
		virtual void play(int) override {};
		virtual void stopAll() override {};
	};
}