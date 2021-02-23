#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>


bool engine::InputManager::ProcessInput()
{
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetKeystroke(0, 0,&m_CurrentState);

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

	for (std::pair<Input, std::vector<std::shared_ptr<BaseCommand>>> command : m_Commands)
	{
		if (command.first.input == m_CurrentState.VirtualKey)
		{
			switch (command.first.triggertype)
			{
			case InputTriggerType::OnInputDown:
				if (m_CurrentState.Flags & XINPUT_KEYSTROKE_KEYDOWN)
					for (int i{ 0 }; i < command.second.size(); i++)
						command.second[i]->Execute();
				break;
			case InputTriggerType::OnInputUp:
				if (m_CurrentState.Flags & XINPUT_KEYSTROKE_KEYUP)
					for (int i{ 0 }; i < command.second.size(); i++)
						command.second[i]->Execute();
				break;
			case InputTriggerType::OnInputHold:
				if (m_CurrentState.Flags & XINPUT_KEYSTROKE_REPEAT)
					for (int i{ 0 }; i < command.second.size(); i++)
						command.second[i]->Execute();
				break;
			}
		}
	}
	
	return true;
}

void engine::InputManager::AddCommand(Input input, const std::shared_ptr<BaseCommand>& command)
{
	m_Commands[input].push_back(command);
}