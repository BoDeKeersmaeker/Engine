#pragma once
#include "Component.h"

class PlayerComponent;
class GridComponent;

namespace engine { class Scene; }

enum class GameMode
{
	Single,
	Coop,
	Versus
};

class GameManagerComponent : public engine::Component
{
public:
	GameManagerComponent(std::shared_ptr<engine::GameObject> owner, std::weak_ptr<engine::Scene> scene, GameMode gameMode,const std::vector<std::string>& levelPaths);

	virtual void Update() override;
	virtual void Render(const engine::Transform& transform) override;

	void SetEnemyCooldown(float greenEnemyCooldown, float purpleEnemyCooldown, float coilyCooldown);
	void NextLevel();
	bool isGameWon() const;

private:
	void LoadLevel(size_t index);
	void LoadSingle();
	void LoadCoop();
	void LoadVersus();
	void SpawnGreenEnemy();
	void SpawnPurpleEnemy();
	void SpawnCoily();
	void ClearEnemies();
	
	std::vector<std::string> m_LevelPaths;
	std::vector<std::weak_ptr<engine::GameObject>> m_pEnemies;
	std::pair<std::weak_ptr<PlayerComponent>, std::weak_ptr<PlayerComponent>> m_pPlayers;
	std::weak_ptr<engine::Scene> m_pScene;
	std::weak_ptr<GridComponent> m_pGrid;
	float m_GreenEnemyCooldown = 3.f;
	float m_CurrentGreenEnemyCooldown = 0.f;
	float m_PurpleEnemyCooldown = 4.f;
	float m_CurrentPurpleEnemyCooldown = 0.f;
	float m_CoilyCooldown = 5.f;
	float m_CurrentCoilyCooldown = 0.f;
	size_t m_CurrentLevelIndex = 0;
	GameMode m_GameMode;
	bool m_GameWon = false;
};
