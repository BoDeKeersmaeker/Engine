#pragma once
#include <memory>
#include "BaseCommand.h"

class PlayerComponent;

namespace engine
{
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