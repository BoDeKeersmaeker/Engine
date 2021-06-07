#include "QbertMenuComponent.h"

#include "GameManagerComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "../3rdParty/imgui-1.81/imgui.h"
#include "SceneManager.h"

QbertMenuComponent::QbertMenuComponent(const std::shared_ptr<engine::GameObject>& owner, const std::weak_ptr<engine::Scene>& scene)
	:Component(owner)
	, m_pScene{ scene }
{
	
}

void QbertMenuComponent::Update()
{
	
}

void QbertMenuComponent::Render(const engine::Transform&)
{
	if(!m_ShowStartMenu)
		return;
	
	ImGui::Begin("Qbert");

	if (m_ShowHowToPlay)
	{
		ImGui::Text("How to play.");
		
		ImGui::Spacing();
		
		ImGui::Text("Player 1: ");
		ImGui::Text("Controller movement:");
		ImGui::Text("Left stick to move or");
		ImGui::Text("Y to move top left");
		ImGui::Text("B to move top right");
		ImGui::Text("X to move bottom left");
		ImGui::Text("A to move bottom right");

		ImGui::Spacing();
		
		ImGui::Text("Keyboard movement:");
		ImGui::Text("Q to move top left");
		ImGui::Text("W to move top right");
		ImGui::Text("A to move bottom left");
		ImGui::Text("S to move bottom right");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		ImGui::Text("Player 2: ");
		ImGui::Text("Controller movement:");
		ImGui::Text("Right stick to move or");
		ImGui::Text("DPAD up to move top left");
		ImGui::Text("DPAD right to move top right");
		ImGui::Text("DPAD left to move bottom left");
		ImGui::Text("DPAD down to move bottom right");

		ImGui::Spacing();
		
		ImGui::Text("Keyboard movement:");
		ImGui::Text("I to move top left");
		ImGui::Text("O to move top right");
		ImGui::Text("K to move bottom left");
		ImGui::Text("L to move bottom right");

		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Understood."))
			m_ShowHowToPlay = !m_ShowHowToPlay;
	}
	else
	{
		ImGui::Text("GameModes");
		if (ImGui::Button("SinglePlayer"))
		{
			m_ShowStartMenu = false;
			auto obj = std::make_shared<engine::GameObject>();
			obj->SetPosition(275.f, 100.f);
			obj->AddComponent<GameManagerComponent>(std::make_shared<GameManagerComponent>(obj, m_pScene, GameMode::Single, std::vector<std::string>{ "./../Data/Level1.txt", "./../Data/Level2.txt", "./../Data/Level3.txt" }));
			m_pScene.lock()->Add(obj);
		}
		if (ImGui::Button("Co-op"))
		{
			m_ShowStartMenu = false;
			auto obj = std::make_shared<engine::GameObject>();
			obj->SetPosition(275.f, 100.f);
			obj->AddComponent<GameManagerComponent>(std::make_shared<GameManagerComponent>(obj, m_pScene, GameMode::Coop, std::vector<std::string>{ "./../Data/Level1.txt", "./../Data/Level2.txt", "./../Data/Level3.txt" }));
			m_pScene.lock()->Add(obj);
		}
		if (ImGui::Button("Versus"))
		{
			m_ShowStartMenu = false;
			auto obj = std::make_shared<engine::GameObject>();
			obj->SetPosition(275.f, 100.f);
			obj->AddComponent<GameManagerComponent>(std::make_shared<GameManagerComponent>(obj, m_pScene, GameMode::Versus, std::vector<std::string>{ "./../Data/Level1.txt", "./../Data/Level2.txt", "./../Data/Level3.txt" }));
			m_pScene.lock()->Add(obj);
		}
	}

	ImGui::End();
}