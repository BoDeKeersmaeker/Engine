#pragma once
#include <memory>

#include "BaseCommand.h"
namespace engine
{
	class PlayerComponent;

	class ColorChange : public engine::BaseCommand
	{
	public:
		ColorChange(std::weak_ptr<PlayerComponent> target);
		virtual ~ColorChange() = default;
		void Execute() override;

	private:
		std::weak_ptr<PlayerComponent> m_pTarget;
	};
}