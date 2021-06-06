#pragma once
#include "Observer.h"

namespace engine{ class CounterComponent; }

class ScoreObserver : public engine::Observer
{
public:
	ScoreObserver(const std::weak_ptr<engine::CounterComponent>& target);

	void OnNotify(engine::Event event, const std::weak_ptr<engine::GameObject>& gameObject) override;
	void OnNotify(engine::Event event, int number) override;

private:
	std::weak_ptr<engine::CounterComponent> m_pTarget = std::shared_ptr<engine::CounterComponent>(nullptr);
	int m_score = 0;
};