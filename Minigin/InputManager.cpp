#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>


bool engine::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) 
	{
		for (std::pair<Input, std::vector<std::shared_ptr<BaseCommand>>> command : m_ControllerCommands)
		{
			if (!command.first.IsKeyboard)
				continue;
			
			if(command.first.input == int(SDL_GetKeyName(e.key.keysym.sym)[0]))
			{
				switch (command.first.triggerType)
				{
				case InputTriggerType::OnInputUp:
					if (e.type & SDL_KEYUP)
						for (size_t i{ 0 }; i < command.second.size(); i++)
							command.second[i]->Execute();
					break;
				case InputTriggerType::OnInputDown:
					if (e.type & SDL_KEYDOWN)
						for (size_t i{ 0 }; i < command.second.size(); i++)
							command.second[i]->Execute();
					break;
				case InputTriggerType::OnInputHold:
					if (e.type & SDL_KEYDOWN)
						for (size_t i{ 0 }; i < command.second.size(); i++)
							command.second[i]->Execute();
					break;
				}
			}
		}
		
		if (e.type == SDL_QUIT)
			return false;
	}

	ZeroMemory(&m_CurrentState, sizeof(XINPUT_KEYSTROKE));

	while(XInputGetKeystroke(0, 0, &m_CurrentState) == ERROR_SUCCESS)
	{
		for (std::pair<Input, std::vector<std::shared_ptr<BaseCommand>>> command : m_ControllerCommands)
		{
			if(command.first.IsKeyboard)
				continue;
			
			if (command.first.input == m_CurrentState.VirtualKey)
			{
				switch (command.first.triggerType)
				{
				case InputTriggerType::OnInputUp:
					if (m_CurrentState.Flags & XINPUT_KEYSTROKE_KEYUP)
						for (size_t i{ 0 }; i < command.second.size(); i++)
							command.second[i]->Execute();
					break;
				case InputTriggerType::OnInputDown:
					if (m_CurrentState.Flags & XINPUT_KEYSTROKE_KEYDOWN && !(m_CurrentState.Flags & XINPUT_KEYSTROKE_REPEAT))
						for (size_t i{ 0 }; i < command.second.size(); i++)
							command.second[i]->Execute();
					break;
				case InputTriggerType::OnInputHold:
					if (m_CurrentState.Flags & XINPUT_KEYSTROKE_KEYDOWN)
						for (size_t i{ 0 }; i < command.second.size(); i++)
							command.second[i]->Execute();
					break;
				}
			}
		}
	}
	return true;
}

void engine::InputManager::AddCommand(int input, InputTriggerType triggerType, const std::shared_ptr<BaseCommand>& command)
{
	DebugManager::GetInstance().print("Controller input added.", INPUT_DEBUG);
	m_ControllerCommands[{input, triggerType, false}].push_back(command);
}

void engine::InputManager::AddCommand(char input, InputTriggerType triggerType, const std::shared_ptr<BaseCommand>& command)
{
	DebugManager::GetInstance().print("Keyboard input added.", INPUT_DEBUG);
	m_ControllerCommands[{input, triggerType, true}].push_back(command);
}