#pragma once
#include <memory>
#include "BaseCommand.h"
#include "Structs.h"


enum class Direction;
class PlayerComponent;

class PlayerMove final : public engine::BaseCommand
{
public:
	PlayerMove(std::weak_ptr<PlayerComponent> target, engine::Direction direction);
	virtual ~PlayerMove()  override = default;
	void Execute() override;

private:
	std::weak_ptr<PlayerComponent> m_pTarget;
	engine::Direction m_Direction;
};