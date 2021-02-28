#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>


bool engine::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
	}

	ZeroMemory(&m_CurrentState, sizeof(XINPUT_KEYSTROKE));

	while(XInputGetKeystroke(0, 0, &m_CurrentState) == ERROR_SUCCESS)
	{
		for (std::pair<Input, std::vector<std::shared_ptr<BaseCommand>>> command : m_Commands)
		{
			if (command.first.input == m_CurrentState.VirtualKey)
			{
				switch (command.first.triggertype)
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

void engine::InputManager::AddCommand(Input input, const std::shared_ptr<BaseCommand>& command)
{
	m_Commands[input].push_back(command);
}