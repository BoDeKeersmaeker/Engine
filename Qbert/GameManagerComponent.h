#pragma once
#include "Component.h"

class CoilyMove;
class ScoreObserver;
class PurpleEnemyComponent;
class GreenEnemyComponent;
class CoilyComponent;
class PlayerComponent;
class GridComponent;

namespace engine {
	class Scene; }

enum class GameMode
{
	Single,
	Coop,
	Versus
};

class GameManagerComponent final : public engine::Component
{
public:
	GameManagerComponent(const std::shared_ptr<engine::GameObject>& owner, const std::weak_ptr<engine::Scene>& scene, GameMode gameMode, const std::vector<std::string>& levelPaths);
	
	void Update() override;
	void Render(const engine::Transform& transform) override;

	void NextLevel();
	bool isGameWon() const;
	int GetScore() const;
	void SetEnemyCooldown(float greenEnemyCooldown = 10.f, float purpleEnemyCooldown = 20.f, float coilyCooldown = 30.f);
	void ClearEnemies();
	void Reset();

private:
	void LoadGameManager(GameMode gameMode);
	void LoadLevel(size_t index);
	void LoadSingle();
	void LoadCoop();
	void LoadVersus();
	void SpawnGreenEnemy();
	void SpawnPurpleEnemy();
	void SpawnCoily();
	void CleanUppEnemies();
	void CheckCharacterOverlap();
	void UpdateCoilyCommands(std::weak_ptr<CoilyComponent> pTarger);
	bool IsGameOver() const;
	
	std::vector<std::string> m_LevelPaths{};
	std::pair<std::weak_ptr<PlayerComponent>, std::weak_ptr<PlayerComponent>> m_pPlayers;
	std::vector<std::weak_ptr<CoilyMove>> m_pCoilyCommands{};
	std::vector<std::weak_ptr<GreenEnemyComponent>> m_pGreenEnemies{};
	std::vector<std::weak_ptr<PurpleEnemyComponent>> m_pPurpleEnemies{};
	std::vector<std::weak_ptr<engine::GameObject>> m_pMiscObjects{};
	std::weak_ptr<CoilyComponent> m_pCoily;
	std::weak_ptr<engine::Scene> m_pScene;
	std::weak_ptr<GridComponent> m_pGrid;
	std::shared_ptr<ScoreObserver> m_pScoreObserver;
	float m_GreenEnemyCooldown{};
	float m_CurrentGreenEnemyCooldown = 0.f;
	float m_PurpleEnemyCooldown{};
	float m_CurrentPurpleEnemyCooldown = 0.f;
	float m_CoilyCooldown{};
	float m_CurrentCoilyCooldown = 0.f;
	int m_Score = 0;
	size_t m_CurrentLevelIndex = 0;
	GameMode m_GameMode;
	bool m_GameWon = false;
	bool m_EnemiesCleared = false;
};