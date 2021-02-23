#pragma once
#include "Singleton.h"

namespace engine
{
	class Time final : public Singleton<Time>
	{
	public:
		inline void UpdateElapsedSec(float elapsedSec) { m_ElapsedSec = elapsedSec; };
		float GetElapsedSec() { return m_ElapsedSec; };

		//static void ChronoStart();
		//static float ChronoStop();

	private:
		friend class Singleton<Time>;
		Time() = default;

		float m_ElapsedSec{};
	};
}
