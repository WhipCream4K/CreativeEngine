#include "pch.h"
#include "InputManager.h"
#include "InputComponent.h"
#include <SDL.h>
#include <Xinput.h>

std::array<bool, 4> dae::InputManager::m_GamepadConnected{}; // first player always connected

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
	if (m_NextFramePassed)
	{
		auto end = std::chrono::high_resolution_clock::now();
		m_ElapsedTime += static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(end - m_StartCount).count());
		m_NextFramePassed = false;
	}
	if (!m_NextFramePassed)
	{
		m_StartCount = std::chrono::high_resolution_clock::now();
		m_NextFramePassed = true;
	}

	auto mouseState = GetMouseState();
	auto keyboardState = GetKeyboardState();
	auto gamepadState = GetGamepadState();

	// reset elapsed time
	m_ElapsedTime = m_ElapsedTime >= m_GamepadStateCheckTimeLimit ?
		m_ElapsedTime - m_GamepadStateCheckTimeLimit :
		m_ElapsedTime;

	// current active player using sign bit
	UINT8 activeController{};
	for (UINT i = 0; i < m_MaxSupportedController; ++i)
	{
		activeController |= UINT8(1 << i * m_GamepadConnected[i]);
	}

	// Action Mapping
	for (auto& group : m_InputActionMappingGroup)
	{
		auto& inputActions{ group.second.GetInputActions() };
		for (auto& action : inputActions)
		{
			if (InputActionExecuteCondition(action, mouseState, keyboardState, gamepadState.data()))
			{
				for (auto& observer : m_Observers)
				{
					if(action.key.gamepadKey != GamePadKey::GPK_None)
					{
						if (activeController & 1 << observer.lock()->GetPlayerIndex())
							observer.lock()->GetActionHandles().at(action.groupName).ShouldExecute();
					}
					else
						observer.lock()->GetActionHandles().at(action.groupName).ShouldExecute();
				}
			}
		}
	}

	// Axis Mapping
	for (auto& group : m_InputAxisMappingGroup)
	{
		auto& inputAxis{ group.second.GetInputAxises() };
		for (auto& axis : inputAxis)
		{
			if (InputAxisExecuteCondition(axis, mouseState, keyboardState, gamepadState.data()))
			{
				for (auto& observer : m_Observers)
				{
					if (axis.key.gamepadKey != GamePadKey::GPK_None)
					{
						if (activeController & 1 << observer.lock()->GetPlayerIndex())
							observer.lock()->GetAxisHandles().at(axis.groupName).ShouldExecute(axis.axisValue);
					}
					else
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

bool dae::InputManager::InputActionExecuteCondition(InputAction& input, const MouseState& mouse, const UINT8* keyboard, const GamepadState* gamepad) const
{
	bool isKeyPressed{};

	switch (input.key.device)
	{
	case Device::D_Keyboard:	isKeyPressed = IsKeyboardKeyPressed(input.key.keyCode, keyboard);		break;
	case Device::D_Mouse:		isKeyPressed = IsMouseKeyPressed(input.key.mouseKey, mouse);			break;
	case Device::D_Gamepad:		isKeyPressed = IsGamepadKeyPressed(input.key.gamepadKey, gamepad);		break;
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

bool dae::InputManager::InputAxisExecuteCondition(InputAxis& input, const MouseState& mouse, const UINT8* keyboard, const GamepadState* gamepad) const
{
	bool isKeyPressed{};

	switch (input.key.device)
	{
	case Device::D_Keyboard:

		isKeyPressed = IsKeyboardKeyPressed(input.key.keyCode, keyboard);
		input.axisValue = isKeyPressed ? input.scaleValue : 0.0f;

		break;
	case Device::D_Mouse:

		isKeyPressed = IsMouseKeyPressed(input.key.mouseKey, mouse, &input.axisValue);
		input.axisValue *= input.scaleValue;

		break;
	case Device::D_Gamepad:

		isKeyPressed = IsGamepadKeyPressed(input.key.gamepadKey, gamepad, &input.axisValue);
		input.axisValue *= input.scaleValue;

		break;
	}

	return isKeyPressed;
}

bool dae::InputManager::IsMouseKeyPressed(MouseKey mouseKey, const MouseState& checkState, float* outAxisValue) const
{
	bool result{};
	if (UINT8(mouseKey) & checkState.mouseKey)
		result = true;
	else
	{
		switch (mouseKey)
		{
		case MouseKey::MK_AxisX:
			result = true;
			*outAxisValue = checkState.x;
			break;
		case MouseKey::MK_AxisY:
			result = true;
			*outAxisValue = checkState.y;
			break;
		default:break;
		}
	}

	return  result;
	//SDL_PumpEvents();
	//int mouseX{}, mouseY{};
	//const uint8_t mouseState{ static_cast<uint8_t>(SDL_GetRelativeMouseState(&mouseX,&mouseY)) };

	//const bool isThisKeyAnAxis{ mouseKey == MouseKey::MK_AxisY || mouseKey == MouseKey::MK_AxisX };
	//*outAxisValue = float(mouseKey == MouseKey::MK_AxisX ? mouseX : mouseY);
	//*outAxisValue = Clamp(*outAxisValue, -1.0f, 1.0f);

	//if (isThisKeyAnAxis && abs(mouseX + mouseY) > 0)
	//	return true;

	//if (uint8_t(mouseKey) & mouseState)
	//	return true;

	//return false;
}

bool dae::InputManager::IsKeyboardKeyPressed(SDL_Keycode keycode, const UINT8* checkState) const
{
	return bool(checkState[SDL_GetScancodeFromKey(keycode)]);
}

bool dae::InputManager::IsGamepadKeyPressed(GamePadKey keycode, const GamepadState* checkState, float* outAxisValue) const
{
	bool result{};
	for (UINT i = 0; i < m_MaxSupportedController; ++i)
	{
		if (checkState[i].isActive)
		{
			result = UINT16(keycode) & checkState[i].gamepadKey;
			if(!result)
			{
				switch (keycode)
				{
				case GamePadKey::GPK_Right_AxisX:
					result = true;
					*outAxisValue = checkState[i].rightThumbX;
					break;
				case GamePadKey::GPK_Right_AxisY:
					result = true;
					*outAxisValue = checkState[i].rightThumbY;
					break;
				case GamePadKey::GPK_Left_AxisX:
					result = true;
					*outAxisValue = checkState[i].leftThumbX;
					break;
				case GamePadKey::GPK_Left_AxisY:
					result = true;
					*outAxisValue = checkState[i].leftThumbY;
					break;
				default:break;
				}
			}
		}
	}

	return result;
}

dae::MouseState dae::InputManager::GetMouseState() const
{
	SDL_PumpEvents();
	MouseState state{};
	int x, y;
	state.mouseKey = static_cast<uint8_t>(SDL_GetRelativeMouseState(&x, &y));
	state.x = float(x);
	state.y = float(y);
	return state;
}

const UINT8* dae::InputManager::GetKeyboardState() const
{
	SDL_PumpEvents();
	//SDL_Keycode key = SDLK_DOWN;
	//auto* keys = SDL_GetKeyboardState(nullptr);
	//bool test = bool(keys[SDL_GetScancodeFromKey(key)]);
	//test;
	return SDL_GetKeyboardState(nullptr);
}

// this function manipulates the elapsedtime of our inputmanager
std::array<dae::GamepadState, dae::InputManager::m_MaxSupportedController> dae::InputManager::GetGamepadState() const
{
	std::array<GamepadState, m_MaxSupportedController> gamepadState{};
	const bool isTimeReached{ m_ElapsedTime >= m_GamepadStateCheckTimeLimit };
	for (DWORD i = 0; i < DWORD(m_MaxSupportedController); ++i)
	{
		XINPUT_STATE state{};
		ZeroMemory(&state, sizeof(XINPUT_STATE));
		
		if (!m_GamepadConnected[i])
		{
			if (!isTimeReached)
				continue;
		}

		auto result = XInputGetState(i, &state);
		if (result == ERROR_SUCCESS)
		{
			// success
			m_GamepadConnected[i] = true;
			gamepadState[i].currentPlayerIndex = UINT8(i);
			gamepadState[i].isActive = true;
			gamepadState[i].gamepadKey = UINT16(state.Gamepad.wButtons);

			bool isOutsideDeadZone{};

			// Left Trigger Dead zone
			isOutsideDeadZone = state.Gamepad.bLeftTrigger > GAMEPAD_LEFT_TRIGGER_DEADZONE;
			state.Gamepad.bLeftTrigger = isOutsideDeadZone ? state.Gamepad.bLeftTrigger - GAMEPAD_LEFT_TRIGGER_DEADZONE : 0;

			gamepadState[i].leftTrigger = float(state.Gamepad.bLeftTrigger) / (UCHAR_MAX - GAMEPAD_LEFT_TRIGGER_DEADZONE);

			// Right Trigger Dead zone
			isOutsideDeadZone = state.Gamepad.bRightTrigger > GAMEPAD_RIGHT_TRIGGER_DEADZONE;
			state.Gamepad.bRightTrigger = isOutsideDeadZone ? state.Gamepad.bRightTrigger - GAMEPAD_RIGHT_TRIGGER_DEADZONE : 0;

			gamepadState[i].rightTrigger = float(state.Gamepad.bRightTrigger) / (UCHAR_MAX - GAMEPAD_RIGHT_TRIGGER_DEADZONE);
			
			//gamepadState[i].leftTrigger = float(state.Gamepad.bLeftTrigger) / 255.0f;
			//gamepadState[i].rightTrigger = float(state.Gamepad.bRightTrigger) / 255.0f;

			// Left Thumb X Dead zone
			isOutsideDeadZone = state.Gamepad.sThumbLX > GAMEPAD_LEFT_THUMBSTICK_X_DEADZONE;
			state.Gamepad.sThumbLX = isOutsideDeadZone ? state.Gamepad.sThumbLX - GAMEPAD_LEFT_THUMBSTICK_X_DEADZONE : 0;
			
			gamepadState[i].leftThumbX = float(state.Gamepad.sThumbLX) / (SHRT_MAX - GAMEPAD_LEFT_THUMBSTICK_X_DEADZONE);

			// Left Thumb Y Dead zone
			isOutsideDeadZone = state.Gamepad.sThumbLY > GAMEPAD_LEFT_THUMBSTICK_Y_DEADZONE;
			state.Gamepad.sThumbLY = isOutsideDeadZone ? state.Gamepad.sThumbLY - GAMEPAD_LEFT_THUMBSTICK_Y_DEADZONE : 0;

			gamepadState[i].leftThumbY = float(state.Gamepad.sThumbLY) / (SHRT_MAX - GAMEPAD_LEFT_THUMBSTICK_Y_DEADZONE);

			// Right Thumb X Dead zone
			isOutsideDeadZone = state.Gamepad.sThumbRX > GAMEPAD_RIGHT_THUMBSTICK_X_DEADZONE;
			state.Gamepad.sThumbRX = isOutsideDeadZone ? state.Gamepad.sThumbRX - GAMEPAD_RIGHT_THUMBSTICK_X_DEADZONE : 0;

			gamepadState[i].rightThumbX = float(state.Gamepad.sThumbRX) / (SHRT_MAX - GAMEPAD_RIGHT_THUMBSTICK_X_DEADZONE);

			// Right Thumb Y Dead zone
			isOutsideDeadZone = state.Gamepad.sThumbRY > GAMEPAD_RIGHT_THUMBSTICK_Y_DEADZONE;
			state.Gamepad.sThumbRY = isOutsideDeadZone ? state.Gamepad.sThumbRY - GAMEPAD_RIGHT_THUMBSTICK_Y_DEADZONE : 0;

			gamepadState[i].rightThumbY = float(state.Gamepad.sThumbRY) / (SHRT_MAX - GAMEPAD_RIGHT_THUMBSTICK_Y_DEADZONE);
			
			//gamepadState[i].leftThumbX = float(state.Gamepad.sThumbLX) / SHRT_MAX;
			//gamepadState[i].leftThumbY = float(state.Gamepad.sThumbLY) / SHRT_MAX;
			//gamepadState[i].rightThumbX = float(state.Gamepad.sThumbRX) / SHRT_MAX;
			//gamepadState[i].rightThumbY = float(state.Gamepad.sThumbRY) / SHRT_MAX;
		}
		else
		{
			// failed (controller not connected)
			m_GamepadConnected[i] = false;
		}
	}

	return gamepadState;
}
