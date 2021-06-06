#pragma once
#include "Component.h"

class QbertMenuComponent final : public engine::Component
{
public:
	QbertMenuComponent(const std::shared_ptr<engine::GameObject>& owner);

	void Update() override;
	void Render(const engine::Transform& transform) override;

	bool IsReadyToPlay() const;
	void SetShowWindow(bool showWindow);

private:
	bool m_ShowHowToPlay = true;
	bool m_ShowStartMenu = true;
};