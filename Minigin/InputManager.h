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
		InputTriggerType triggerType;
		bool IsKeyboard;

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
			return input == temp.input && triggerType == temp.triggerType && IsKeyboard == temp.IsKeyboard;
		}
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		
		void AddControllerCommand(int input, InputTriggerType triggerType, const std::shared_ptr<BaseCommand>& command);
		void AddKeyBoardCommand(int input, InputTriggerType triggerType, const std::shared_ptr<BaseCommand>& command);

		void Reset();
	
	private:
		XINPUT_KEYSTROKE m_CurrentState{};
		std::map <Input, std::vector<std::shared_ptr<BaseCommand>>> m_ControllerCommands;
	};
}