#include "QbertMenuComponent.h"
//#include "../3rdParty/imgui-1.81/imgui.h"
//#include <../3rdParty/imgui-1.81/backends/imgui_impl_opengl2.h>
//#include <../3rdParty/imgui-1.81/backends/imgui_impl_sdl.h>

#include "imgui.h"
#include <backends/imgui_impl_opengl2.h>
#include <backends/imgui_impl_sdl.h>

#include "SceneManager.h"

QbertMenuComponent::QbertMenuComponent(const std::shared_ptr<engine::GameObject>& owner)
	:Component(owner)
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
			m_ShowStartMenu = false;
		if (ImGui::Button("Co-op"))
			m_ShowStartMenu = false;
		if (ImGui::Button("Versus"))
			m_ShowStartMenu = false;
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

bool QbertMenuComponent::IsReadyToPlay() const
{
	return !m_ShowStartMenu;
}

void QbertMenuComponent::SetShowWindow(bool showWindow)
{
	m_ShowStartMenu = showWindow;
	m_ShowHowToPlay = showWindow;
}