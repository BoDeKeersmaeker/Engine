#pragma once
#include "Singleton.h"

namespace engine
{
	class EngineTime final : public Singleton<EngineTime>
	{
	public:
		void UpdateElapsedSec(float elapsedSec) { m_ElapsedSec = elapsedSec; };
		float GetElapsedSec() const { return m_ElapsedSec; };

	private:
		friend class Singleton<EngineTime>;
		EngineTime() = default;

		float m_ElapsedSec{};
	};
}