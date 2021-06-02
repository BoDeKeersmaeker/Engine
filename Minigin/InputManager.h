#pragma once
#include <XInput.h>
#pragma comment(lib, "XInput.lib")
#include "Singleton.h"
#include <map>
#include <vector>
#include "BaseCommand.h"
#include <memory>

namespace engine
{
	enum class InputTriggerType
	{
		OnInputDown,
		OnInputUp,
		OnInputHold
	};
	struct Input
	{
		int input;
		InputTriggerType triggertype;

		bool operator< (const Input& temp) const
		{
			return input < temp.input;
		}

		bool operator> (const Input& temp) const
		{
			return input > temp.input;
		}

		bool operator== (const Input& temp) const
		{
			return input == temp.input && triggertype == temp.triggertype;
		}
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		
		void AddCommand(Input input, const std::shared_ptr<BaseCommand>& command);
	
	private:
		XINPUT_KEYSTROKE m_CurrentState{};
		std::map <Input, std::vector<std::shared_ptr<BaseCommand>>> m_Commands;
	};
}