#pragma once
#include <memory>
#include "BaseCommand.h"
#include "Structs.h"

enum class Direction;
class CoilyComponent;

class CoilyMove final : public engine::BaseCommand
{
public:
	CoilyMove(std::weak_ptr<CoilyComponent> target, engine::Direction direction);
	virtual ~CoilyMove()  override = default;
	void Execute() override;
	void ChangeTarget(std::weak_ptr<CoilyComponent> target);

private:
	std::weak_ptr<CoilyComponent> m_pTarget;
	engine::Direction m_Direction;
};