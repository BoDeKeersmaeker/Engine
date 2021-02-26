#pragma once
#include "BaseCommand.h"

namespace engine
{
	class PlayerComponent;

	class CatchSlick : public engine::BaseCommand
	{
	public:
		CatchSlick(std::weak_ptr<PlayerComponent> target);
		virtual ~CatchSlick() = default;
		void Execute() override;

	private:
		std::weak_ptr<PlayerComponent> m_pTarget;
	};
};