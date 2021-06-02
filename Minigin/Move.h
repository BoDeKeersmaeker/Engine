#pragma once
#include <memory>
#include "BaseCommand.h"

namespace engine
{
	enum class MoveDirection;
	class PlayerComponent;

	class Move final : public engine::BaseCommand
	{
	public:
		Move(std::weak_ptr<PlayerComponent> target, MoveDirection direction);
		virtual ~Move()  override = default;
		void Execute() override;

	private:
		std::weak_ptr<PlayerComponent> m_pTarget;
		MoveDirection m_Direction;
	};
};