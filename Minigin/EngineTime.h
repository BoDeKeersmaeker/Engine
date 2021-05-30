#pragma once
#include "Singleton.h"

namespace engine
{
	class EngineTime final : public Singleton<EngineTime>
	{
	public:
		inline void UpdateElapsedSec(float elapsedSec) { m_ElapsedSec = elapsedSec; };
		float GetElapsedSec() { return m_ElapsedSec; };

		//static void ChronoStart();
		//static float ChronoStop();

	private:
		friend class Singleton<EngineTime>;
		EngineTime() = default;

		float m_ElapsedSec{};
	};
}
