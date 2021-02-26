#pragma once
#include "BaseCommand.h"

namespace engine
{
	class PlayerComponent;

	class CoilyDiscBait : public engine::BaseCommand
	{
	public:
		CoilyDiscBait(std::weak_ptr<PlayerComponent> target);
		virtual ~CoilyDiscBait() = default;
		void Execute() override;

	private:
		std::weak_ptr<PlayerComponent> m_pTarget;
	};
};