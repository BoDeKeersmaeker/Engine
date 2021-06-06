#pragma once
#include <memory>
#include "BaseCommand.h"
#include "Helpers.h"

enum class Direction;
class CoilyComponent;

class CoilyMove final : public engine::BaseCommand
{
public:
	CoilyMove(std::weak_ptr<CoilyComponent> target, Direction direction);
	virtual ~CoilyMove()  override = default;
	void Execute() override;
	void ChangeTarget(std::weak_ptr<CoilyComponent> target);

private:
	std::weak_ptr<CoilyComponent> m_pTarget;
	Direction m_Direction;
};