#pragma once
#include <memory>

#include "BaseCommand.h"

namespace engine
{
	class PlayerComponent;

	class CatchSam : public engine::BaseCommand
	{
	public:
		CatchSam(std::weak_ptr<PlayerComponent> target);
		virtual ~CatchSam() = default;
		void Execute() override;

	private:
		std::weak_ptr<PlayerComponent> m_pTarget;
	};
};