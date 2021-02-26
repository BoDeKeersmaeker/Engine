#pragma once
#include "Observer.h"

namespace engine
{
	class ScoreComponent;

class PlayerScoreObserver : public Observer
{
public:
	PlayerScoreObserver(std::weak_ptr<ScoreComponent> target);

	void OnNotify(std::weak_ptr<GameObject> gameObject, Event event) override;

private:
	std::weak_ptr<ScoreComponent> m_pTarget = std::shared_ptr<ScoreComponent>(nullptr);
};
};

