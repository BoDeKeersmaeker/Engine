#pragma once
#include <memory>
#include "BaseCommand.h"
#include "Helpers.h"

class PlayerComponent;

class PlayerMove final : public engine::BaseCommand
{
public:
	PlayerMove(const std::weak_ptr<PlayerComponent>& target, Direction direction);
	~PlayerMove()  override = default;

	PlayerMove(const PlayerMove& other) = delete;
	PlayerMove(PlayerMove&& other) = delete;
	PlayerMove& operator=(const PlayerMove& other) = delete;
	PlayerMove& operator=(PlayerMove&& other) = delete;
	
	void Execute() override;

private:
	std::weak_ptr<PlayerComponent> m_pTarget;
	Direction m_Direction;
};