#pragma once
#include "Observer.h"

class GameManagerComponent;

namespace engine { class CounterComponent; }

class LifeObserver final : public engine::Observer
{
public:
	LifeObserver(const std::weak_ptr<engine::CounterComponent>& pTarget, const std::weak_ptr<engine::GameObject>& pManager = std::shared_ptr<engine::GameObject>(nullptr));

	void OnNotify(engine::Event event, const std::weak_ptr<engine::GameObject>& gameObject) override;
	void OnNotify(engine::Event event, int number) override;

private:
	std::weak_ptr<engine::CounterComponent> m_pTarget = std::shared_ptr<engine::CounterComponent>(nullptr);
	std::weak_ptr<engine::GameObject> m_pManager;
	int m_score = 0;
};