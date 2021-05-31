#include "MiniginPCH.h"
#include "TextComponent.h"
#include "Transform.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdexcept>

engine::TextComponent::TextComponent(std::shared_ptr<GameObject> owner, const std::string& text, const std::shared_ptr<Font>& font)
	:Component(owner)
	, m_NeedsUpdate(true)
	, m_Text(text)
	, m_Font(font)
	, m_Texture(nullptr)
	, m_TextColor{ 255,255,255 }
{

}

void engine::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		const SDL_Color color = { Uint8(m_TextColor.R), Uint8(m_TextColor.G), Uint8(m_TextColor.B) };
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_Texture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void engine::TextComponent::Render(const engine::Transform& transform)
{
	if (m_Texture != nullptr)
	{
		const auto pos = transform.GetPosition();
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}

void engine::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}