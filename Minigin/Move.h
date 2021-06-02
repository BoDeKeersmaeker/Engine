#pragma once
#include <memory>
#include "BaseCommand.h"

namespace engine
{
	enum class Direction;
	class PlayerComponent;

	class Move final : public engine::BaseCommand
	{
	public:
		Move(std::weak_ptr<PlayerComponent> target, Direction direction);
		virtual ~Move()  override = default;
		void Execute() override;

	private:
		std::weak_ptr<PlayerComponent> m_pTarget;
		Direction m_Direction;
	};
};