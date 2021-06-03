#pragma once
#include <memory>
#include "BaseCommand.h"

class PlayerComponent;

namespace engine
{
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

