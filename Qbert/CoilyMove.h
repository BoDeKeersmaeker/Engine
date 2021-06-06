#pragma once
#include <memory>
#include "BaseCommand.h"
#include "Helpers.h"

enum class Direction;
class CoilyComponent;

class CoilyMove final : public engine::BaseCommand
{
public:
	CoilyMove(const std::weak_ptr<CoilyComponent>& target, Direction direction);
	~CoilyMove()  override = default;

	CoilyMove(const CoilyMove& other) = delete;
	CoilyMove(CoilyMove&& other) = delete;
	CoilyMove& operator=(const CoilyMove& other) = delete;
	CoilyMove& operator=(CoilyMove&& other) = delete;
	
	void Execute() override;
	void ChangeTarget(const std::weak_ptr<CoilyComponent>& target);

private:
	std::weak_ptr<CoilyComponent> m_pTarget;
	Direction m_Direction;
};