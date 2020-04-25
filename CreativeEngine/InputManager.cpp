#include "pch.h"
#include "InputManager.h"
#include "EngineContext.h"

#include <SDL.h>

void dae::InputActionMappingGroup::AddKey(const Key& key, InputEvent inputEvent)
{
	InputAction inputAction{ key,m_GroupName,inputEvent };
	m_InputActionLists.emplace_back(inputAction);
}

void dae::InputAxisMappingGroup::AddKey(const Key& key, float axisValue)
{
	const float clampAxis{ Clamp(axisValue,-1.0f,1.0f) };
	InputAxis inputAxis{ key,m_GroupName,clampAxis };
	m_InputAxisLists.emplace_back(inputAxis);
}

dae::InputActionMappingGroup& dae::InputManager::AddInputActionGroup(const std::string& groupName)
{
	return m_InputActionMappingGroup[groupName];
}

dae::InputAxisMappingGroup& dae::InputManager::AddInputAxisGroup(const std::string& groupName)
{
	return m_InputAxisMappingGroup[groupName];
}

dae::InputActionMappingGroup& dae::InputManager::GetActionMappingGroup(const std::string& groupName)
{
	return m_InputActionMappingGroup.at(groupName);
}

dae::InputAxisMappingGroup& dae::InputManager::GetAxisMappingGroup(const std::string& groupName)
{
	return m_InputAxisMappingGroup.at(groupName);
}

void dae::InputManager::ReadInputs()
{
	SDL_PumpEvents();

	for (auto& group : m_InputActionMappingGroup)
	{
		auto& inputActions{ group.second.GetInputActions() };
		for (auto& action : inputActions)
		{
			InputActionExecuteCondition(action);
		}
	}

	for (auto group : m_InputAxisMappingGroup)
	{
		auto& inputAxis{ group.second.GetInputAxises() };
		for (auto axis : inputAxis)
		{
			InputAxisExecuteCondition(axis);
		}
	}
}

void dae::InputManager::InputActionExecuteCondition(InputAction& input) const
{	
	bool isKeyPressed{};

	switch (input.key.device)
	{
	case Device::D_Keyboard:
		isKeyPressed = IsKeyboardKeyPressed(input.key.keyCode);
		break;
	case Device::D_Mouse:
		isKeyPressed = IsMouseKeyPressed(input.key.mouseKey);
		break;
	case Device::D_Gamepad:
		break;
	default: ;
	}

	switch (input.inputEvent)
	{
	case InputEvent::IE_Pressed:

		if (isKeyPressed)
		{
			if (input.isPressedFlag)
			{
				input.isTriggered = false;
				break;
			}

			input.isPressedFlag = true;
			input.isTriggered = true;
		}
		else
			input.isPressedFlag = false;
		

		break;
	case InputEvent::IE_Released:

		if (isKeyPressed)
			input.isPressedFlag = true;

		if (!isKeyPressed)
		{
			input.isTriggered = true;
			input.isPressedFlag = false;
		}

		break;
	case InputEvent::IE_Down:

		input.isTriggered = isKeyPressed ? true : false;

		break;
	default:
		break;
	}
}

void dae::InputManager::InputAxisExecuteCondition(InputAxis& input) const
{
	bool isKeyPressed{};

	switch (input.key.device)
	{
	case Device::D_Keyboard:

		isKeyPressed = IsKeyboardKeyPressed(input.key.keyCode);
		input.axisValue = isKeyPressed ? input.scaleValue : 0.0f;

		break;
	case Device::D_Mouse:
	
		IsMouseKeyPressed(input.key.mouseKey, &input.axisValue);
		input.axisValue *= input.scaleValue;
		
		break;
	case Device::D_Gamepad:
		break;
	default: ;
	}
}

bool dae::InputManager::IsMouseKeyPressed(MouseKey mouseKey,float* axisValue) const
{
	int mouseX{}, mouseY{};
	const uint8_t mouseState{ static_cast<uint8_t>(SDL_GetRelativeMouseState(&mouseX,&mouseY)) };
	
	const bool isThisKeyAnAxis{ mouseKey == MouseKey::MK_AxisY || mouseKey == MouseKey::MK_AxisX };
	*axisValue = float(mouseKey == MouseKey::MK_AxisX ? mouseX : mouseY);
	*axisValue = Clamp(*axisValue, -1.0f,1.0f);

	if (isThisKeyAnAxis && abs(mouseX + mouseY) > 0)
		return true;
	
	if (uint8_t(mouseKey) == mouseState && mouseState != 0)
		return true;
	
	return false;
}

bool dae::InputManager::IsKeyboardKeyPressed(SDL_Keycode keycode) const
{
	const uint8_t* keyboardState{ static_cast<const uint8_t*>(SDL_GetKeyboardState(nullptr)) };
	return bool(keyboardState[keycode]);
}
