#pragma once
#include <memory>
#include "BaseCommand.h"

namespace engine
{
	class PlayerComponent;
	
	class Kill final : public engine::BaseCommand
	{
	public:
		Kill(std::weak_ptr<PlayerComponent> target);
		virtual ~Kill() = default;
		void Execute() override;

	private:
		std::weak_ptr<PlayerComponent> m_pTarget;
	};
}

