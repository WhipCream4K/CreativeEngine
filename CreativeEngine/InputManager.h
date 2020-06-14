#pragma once
#include <string>
#include <unordered_map>
#include "SDL.h"
#include <vector>
#include <glm.hpp>

namespace dae
{
	enum class InputEvent
	{
		IE_Pressed,
		IE_Released,
		IE_Down
	};

	// my enum for mouse key since 
	enum class MouseKey
	{
		MK_None,
		MK_Left,
		MK_Middle,
		MK_Right,
		MK_Thumb1,
		MK_Thumb2,
		MK_AxisX,
		MK_AxisY
	};

	enum class Device
	{
		D_Keyboard,
		D_Mouse,
		D_Gamepad
	};

	struct Key
	{
		// TODO: Add gamepad keys
		Key()
			: device{ Device::D_Keyboard }
			, keyCode{ -1 }
			, mouseKey{ MouseKey::MK_None }
		{
		}

		/**
		 * \brief
		 * \param device an enum class of Device e.g D_Keyboard,D_Mouse
		 * \param SDL_KeyboardCode keyboard code value type of uint8
		 * \param SDL_Mouse a SDL mouse macro e.g SDL_BUTTON_LEFT, SDL_MOUSE_MOTION_X
		 */
		Key(Device device, SDL_Keycode SDL_KeyboardCode, MouseKey SDL_Mouse)
			: device{ device }
			, keyCode{ SDL_KeyboardCode }
			, mouseKey{ SDL_Mouse }
		{
		}

		bool operator==(const Key& other) const
		{
			return (device == other.device &&
				(keyCode == other.keyCode || mouseKey == other.mouseKey));
		}

		Device device;
		SDL_Keycode keyCode;
		MouseKey mouseKey;
	};

	struct InputAction
	{

		InputAction(const Key& _key, const std::string& _groupName, InputEvent _inputEvent)
			: key{ _key }
			, groupName{ _groupName }
			, inputEvent{ _inputEvent }
			, isTriggered{}
			, isPressedFlag{}
		{
		}

		Key key;
		std::string groupName;
		InputEvent inputEvent;
		bool isTriggered;
		bool isPressedFlag; // use for press event to detect if the button still pressed in previous frame
	};

	struct InputAxis
	{

		InputAxis(const Key& _key, const std::string& _groupName, float _scaleValue)
			: key{ _key }
			, groupName{ _groupName }
			, scaleValue{ _scaleValue }
			, axisValue{}
		{
		}

		Key key;
		std::string groupName;
		float scaleValue;
		float axisValue; // actual value when user has pressed the key
	};

	class InputActionMappingGroup final
	{
	public:
		const std::string& GetGroupName() const { return m_GroupName; }
		void AddKey(const Key& key, InputEvent inputEvent = InputEvent::IE_Pressed);
		std::vector<InputAction>& GetInputActions() { return m_InputActionLists; }
	private:
		std::string m_GroupName;
		std::vector<InputAction> m_InputActionLists;
	};

	class InputAxisMappingGroup final
	{
	public:
		const std::string& GetGroupName() const { return m_GroupName; }
		void AddKey(const Key& key, float axisValue);
		std::vector<InputAxis>& GetInputAxises() { return m_InputAxisLists; }
	private:
		std::string m_GroupName;
		std::vector<InputAxis> m_InputAxisLists;
	};

	class InputManager final
	{
	public:

		InputManager() = default;

		InputActionMappingGroup& AddInputActionGroup(const std::string& groupName);
		InputAxisMappingGroup& AddInputAxisGroup(const std::string& groupName);

		InputActionMappingGroup& GetActionMappingGroup(const std::string& groupName);
		InputAxisMappingGroup& GetAxisMappingGroup(const std::string& groupName);

		// Captures all the player inputs
		void ReadInputs();

	private:

		std::unordered_map<std::string, InputActionMappingGroup> m_InputActionMappingGroup;
		std::unordered_map<std::string, InputAxisMappingGroup> m_InputAxisMappingGroup;

		void InputActionExecuteCondition(InputAction& input) const;
		void InputAxisExecuteCondition(InputAxis& input) const;
		bool IsMouseKeyPressed(MouseKey mouseKey,float* axisValue = nullptr) const;
		bool IsKeyboardKeyPressed(SDL_Keycode keycode) const;
	};
}

