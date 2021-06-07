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
		ImGui::Text("Player 1: ");
		ImGui::Text("Right shoulder to die.");
		ImGui::Text("A to change color.");
		ImGui::Text("B to bait coily of the edge.");
		ImGui::Text("X to Catch Slick.");
		ImGui::Text("Y to Catch Sam.");

		ImGui::Spacing();

		ImGui::Text("Player 2: ");
		ImGui::Text("Left shoulder to die.");
		ImGui::Text("DPAD down to change color.");
		ImGui::Text("DPAD right to bait coily of the edge.");
		ImGui::Text("DPAD left to Catch Slick.");
		ImGui::Text("DPAD up to Catch Sam.");

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