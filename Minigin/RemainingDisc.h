#pragma once
#include <memory>
#include "BaseCommand.h"

namespace engine
{
	class PlayerComponent;

	class RemainingDisc : public engine::BaseCommand
	{
	public:
		RemainingDisc(std::weak_ptr<PlayerComponent> target);
		virtual ~RemainingDisc() = default;
		void Execute() override;

	private:
		std::weak_ptr<PlayerComponent> m_pTarget;
	};
};
