#pragma once
#include <string>
#include <unordered_map>
#include "SDL.h"
#include <vector>

namespace dae
{
	enum class InputEvent
	{
		IE_Pressed,
		IE_Released,
		IE_Down
	};

	enum class KeyState
	{
		K_None,
		K_NotChange,
		K_Pressed,
		K_Release,
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
		Key(Device device, SDL_Keycode SDL_KeyboardCode, MouseKey SDL_Mouse = MouseKey::MK_None)
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

		InputAction(const Key& defKey, const std::string& groupName/*, InputEvent event*/)
			: key{ defKey }
			, groupName{ groupName }
			, keyState{ KeyState::K_None }
			//, inputEvent{ event }
			//, isTriggered{}
			//, isPressedFlag{}
		{
		}

		Key key;
		std::string groupName;
		KeyState keyState;
		//InputEvent inputEvent;
		//bool isTriggered;
		//bool isPressedFlag; // use for press event to detect if the button still pressed in previous frame
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
		InputActionMappingGroup(const std::string& groupName);
		const std::string& GetGroupName() const { return m_GroupName; }
		void AddKey(const Key& key);
		std::vector<InputAction>& GetInputActions() { return m_InputActionLists; }
	private:
		std::string m_GroupName;
		std::vector<InputAction> m_InputActionLists;
	};

	class InputAxisMappingGroup final
	{
	public:
		InputAxisMappingGroup(const std::string& groupName);
		const std::string& GetGroupName() const { return m_GroupName; }
		void AddKey(const Key& key, float axisValue);
		std::vector<InputAxis>& GetInputAxises() { return m_InputAxisLists; }
	private:
		std::string m_GroupName;
		std::vector<InputAxis> m_InputAxisLists;
	};

	class InputComponent;
	class InputManager final
	{
	public:

		InputManager();

		InputActionMappingGroup& AddInputActionGroup(const std::string& groupName);
		InputAxisMappingGroup& AddInputAxisGroup(const std::string& groupName);

		InputActionMappingGroup& GetActionMappingGroup(const std::string& groupName);
		InputAxisMappingGroup& GetAxisMappingGroup(const std::string& groupName);

		// Captures all the player inputs
		void ReadInputs();
		auto RegisterObserver(std::weak_ptr<InputComponent>&& inputComponent) -> void;

	private:

		std::unordered_map<std::string, InputActionMappingGroup> m_InputActionMappingGroup;
		std::unordered_map<std::string, InputAxisMappingGroup> m_InputAxisMappingGroup;

		std::vector<std::weak_ptr<InputComponent>> m_Observers;

		bool InputActionExecuteCondition(InputAction& input) const;
		bool InputAxisExecuteCondition(InputAxis& input) const;
		bool IsMouseKeyPressed(MouseKey mouseKey, float* axisValue = nullptr) const;
		bool IsKeyboardKeyPressed(SDL_Keycode keycode) const;
	};
}

