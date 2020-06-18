#include "pch.h"
#include "InputManager.h"
#include "EngineContext.h"
#include "InputComponent.h"
#include <SDL.h>

dae::InputActionMappingGroup::InputActionMappingGroup(const std::string& groupName)
	: m_GroupName{ groupName }
{
}

void dae::InputActionMappingGroup::AddKey(const Key& key)
{
	InputAction inputAction{ key,m_GroupName };
	m_InputActionLists.emplace_back(inputAction);
}

dae::InputAxisMappingGroup::InputAxisMappingGroup(const std::string& groupName)
	: m_GroupName{ groupName }
{
}

void dae::InputAxisMappingGroup::AddKey(const Key& key, float axisValue)
{
	const float clampAxis{ Clamp(axisValue,-1.0f,1.0f) };
	InputAxis inputAxis{ key,m_GroupName,clampAxis };
	m_InputAxisLists.emplace_back(inputAxis);
}

dae::InputManager::InputManager()
	: m_InputActionMappingGroup{}
	, m_InputAxisMappingGroup{}
	, m_Observers{}
{
}

dae::InputActionMappingGroup& dae::InputManager::AddInputActionGroup(const std::string& groupName)
{
	return m_InputActionMappingGroup.try_emplace(groupName, InputActionMappingGroup(groupName)).first->second;
}

dae::InputAxisMappingGroup& dae::InputManager::AddInputAxisGroup(const std::string& groupName)
{
	return m_InputAxisMappingGroup.try_emplace(groupName, InputAxisMappingGroup(groupName)).first->second;
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

	for (auto& group : m_InputActionMappingGroup)
	{
		auto& inputActions{ group.second.GetInputActions() };
		for (auto& action : inputActions)
		{
			if (InputActionExecuteCondition(action))
			{
				for (auto& observer : m_Observers)
				{
					observer.lock()->GetActionHandles().at(action.groupName).ShouldExecute();
				}
			}
		}
	}

	for (auto& group : m_InputAxisMappingGroup)
	{
		auto& inputAxis{ group.second.GetInputAxises() };
		for (auto& axis : inputAxis)
		{
			if (InputAxisExecuteCondition(axis))
			{
				for (auto& observer : m_Observers)
				{
					observer.lock()->GetAxisHandles().at(axis.groupName).ShouldExecute(axis.axisValue);
				}
			}
		}
	}
}

auto dae::InputManager::RegisterObserver(std::weak_ptr<InputComponent>&& inputComponent) -> void
{
	m_Observers.emplace_back(std::move(inputComponent));
}

bool dae::InputManager::InputActionExecuteCondition(InputAction& input) const
{
	bool isKeyPressed{};

	switch (input.key.device)
	{
	case Device::D_Keyboard: isKeyPressed = IsKeyboardKeyPressed(input.key.keyCode); break;
	case Device::D_Mouse: isKeyPressed = IsMouseKeyPressed(input.key.mouseKey); break;
	case Device::D_Gamepad: break;
	default:;
	}

	switch (input.keyState) {
	case KeyState::K_None:

		if (isKeyPressed)
			input.keyState = KeyState::K_Pressed;

		break;

	case KeyState::K_NotChange:

		if (!isKeyPressed)
			input.keyState = KeyState::K_Release;

		break;
	case KeyState::K_Pressed:

		input.keyState = isKeyPressed ?
			KeyState::K_NotChange :
			KeyState::K_Release;

		break;
	case KeyState::K_Release:

		input.keyState = isKeyPressed ?
			KeyState::K_Pressed :
			KeyState::K_None;

		break;
	default:;
	}

	return input.keyState == KeyState::K_Release || input.keyState == KeyState::K_Pressed;

#pragma region Possibly Garbage
	//switch (input.inputEvent)
	//{
	//case InputEvent::IE_Pressed:

	//	if (isKeyPressed)
	//	{
	//		if (input.isPressedFlag)
	//		{
	//			input.isTriggered = false;
	//			break;
	//		}

	//		input.isPressedFlag = true;
	//		input.isTriggered = true;
	//	}
	//	else
	//		input.isPressedFlag = false;
	//	
	//	
	//	break;
	//case InputEvent::IE_Released:

	//	if (isKeyPressed)
	//		input.isPressedFlag = true;

	//	if (!isKeyPressed)
	//	{
	//		input.isTriggered = true;
	//		input.isPressedFlag = false;
	//	}

	//	break;
	//case InputEvent::IE_Down:

	//	input.isTriggered = isKeyPressed ? true : false;

	//	break;
	//default:
	//	break;
	//}
#pragma endregion 
}

bool dae::InputManager::InputAxisExecuteCondition(InputAxis& input) const
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
	default:;
	}

	return isKeyPressed;
}

bool dae::InputManager::IsMouseKeyPressed(MouseKey mouseKey, float* axisValue) const
{
	SDL_PumpEvents();
	int mouseX{}, mouseY{};
	const uint8_t mouseState{ static_cast<uint8_t>(SDL_GetRelativeMouseState(&mouseX,&mouseY)) };

	const bool isThisKeyAnAxis{ mouseKey == MouseKey::MK_AxisY || mouseKey == MouseKey::MK_AxisX };
	*axisValue = float(mouseKey == MouseKey::MK_AxisX ? mouseX : mouseY);
	*axisValue = Clamp(*axisValue, -1.0f, 1.0f);

	if (isThisKeyAnAxis && abs(mouseX + mouseY) > 0)
		return true;

	if (uint8_t(mouseKey) == mouseState && mouseState != 0)
		return true;

	return false;
}

bool dae::InputManager::IsKeyboardKeyPressed(SDL_Keycode keycode) const
{
	SDL_PumpEvents();
	const uint8_t* keyboardState{ static_cast<const uint8_t*>(SDL_GetKeyboardState(nullptr)) };
	return bool(keyboardState[SDL_GetScancodeFromKey(keycode)]);
}
