#pragma once
#include <memory>
#include "BaseCommand.h"
#include "Helpers.h"

class PlayerComponent;

class PlayerMove final : public engine::BaseCommand
{
public:
	PlayerMove(std::weak_ptr<PlayerComponent> target, Direction direction);
	virtual ~PlayerMove()  override = default;
	void Execute() override;

private:
	std::weak_ptr<PlayerComponent> m_pTarget;
	Direction m_Direction;
};