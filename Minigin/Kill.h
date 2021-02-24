#pragma once
#include "BaseCommand.h"
namespace engine
{
	class GameObject;
	
	class Kill : public engine::BaseCommand
	{
	public:
		Kill(std::shared_ptr<GameObject> target);
		virtual ~Kill() = default;
		void Execute() override;

	private:
		std::shared_ptr<GameObject> m_pTarget;
	};
}

