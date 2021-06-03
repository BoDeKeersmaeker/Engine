#pragma once
#include <memory>
#include "BaseCommand.h"
#include "Structs.h"


enum class Direction;
class PlayerComponent;

class Move final : public engine::BaseCommand
{
public:
	Move(std::weak_ptr<PlayerComponent> target, engine::Direction direction);
	virtual ~Move()  override = default;
	void Execute() override;

private:
	std::weak_ptr<PlayerComponent> m_pTarget;
	engine::Direction m_Direction;
};