#pragma once
#include <string>
#include <unordered_map>
#include "SDL.h"
#include <array>
#include <Windows.h>
#include <vector>

namespace dae
{
	constexpr SHORT GAMEPAD_LEFT_THUMBSTICK_Y_DEADZONE{ 7849 };
	constexpr SHORT GAMEPAD_LEFT_THUMBSTICK_X_DEADZONE{ 7849 };
	constexpr SHORT GAMEPAD_RIGHT_THUMBSTICK_Y_DEADZONE{ 8689 };
	constexpr SHORT GAMEPAD_RIGHT_THUMBSTICK_X_DEADZONE{ 8689 };
	constexpr BYTE	GAMEPAD_LEFT_TRIGGER_DEADZONE{ 30 };
	constexpr BYTE	GAMEPAD_RIGHT_TRIGGER_DEADZONE{ 30 };

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
	enum class MouseKey : UINT8
	{
		MK_None = 0,
		MK_Left = (1 << 0),
		MK_Middle = (1 << 1),
		MK_Right = (1 << 2),
		MK_Thumb1 = (1 << 3),
		MK_Thumb2 = (1 << 4),
		MK_AxisX,
		MK_AxisY
	};

	enum class GamePadKey : UINT16
	{
		GPK_None = 0,
		GPK_DPAD_Up = (1 << 0),
		GPK_DPAD_Down = (1 << 1),
		GPK_DPAD_Left = (1 << 2),
		GPK_DPAD_Right = (1 << 3),
		GPK_Start = (1 << 4),
		GPK_Back = (1 << 5),
		GPK_Left_Thumb = (1 << 6),
		GPK_Right_Thumb = (1 << 7),
		GPK_Left_Shoulder = (1 << 8),
		GPK_Right_Shoulder = (1 << 9),
		GPK_A = (1 << 12),
		GPK_B = (1 << 13),
		GPK_X = (1 << 14),
		GPK_Y = (1 << 15),
		GPK_Right_AxisX = (1 << 10),
		GPK_Right_AxisY,
		GPK_Left_AxisX,
		GPK_Left_AxisY
	};

	enum class Device
	{
		D_Keyboard,
		D_Mouse,
		D_Gamepad
	};

	enum class ActiveGamepadBit : UINT8
	{
		Player_0 = (1 << 0),
		Player_1 = (1 << 1),
		Player_2 = (1 << 2),
		Player_3 = (1 << 3)
	};

	struct SupportedKey
	{
		~SupportedKey() = default;
		virtual bool IsKeyPressed() = 0;
		virtual float KeyValue() {}
	};

	struct KeyBoardKey : SupportedKey
	{
		bool IsKeyPressed() override {}
	};

	struct Key
	{
		Key()
			: device{ Device::D_Keyboard }
			, keyCode{ -1 }
			, mouseKey{ MouseKey::MK_None }
			, gamepadKey{ GamePadKey::GPK_None }
		{
		}

		/**
		 * \brief
		 * \param device an enum class of Device e.g D_Keyboard,D_Mouse
		 * \param SDL_KeyboardCode keyboard code value type of uint8
		 * \param SDL_Mouse a SDL mouse macro e.g SDL_BUTTON_LEFT, SDL_MOUSE_MOTION_X
		 */
		Key(Device device, SDL_Keycode SDL_KeyboardCode = -1, MouseKey SDL_Mouse = MouseKey::MK_None, GamePadKey XinputKey = GamePadKey::GPK_None)
			: device{ device }
			, keyCode{ SDL_KeyboardCode }
			, mouseKey{ SDL_Mouse }
			, gamepadKey{ XinputKey }
		{
		}

		bool operator==(const Key& other) const
		{
			return (device == other.device &&
				(keyCode == other.keyCode
					|| mouseKey == other.mouseKey
					|| gamepadKey == other.gamepadKey));
		}

		Device device;
		SDL_Keycode keyCode;
		MouseKey mouseKey;
		GamePadKey gamepadKey;
	};

	struct InputAction
	{

		InputAction(const Key& defKey, const std::string& name/*, InputEvent event*/)
			: key{ defKey }
			, groupName{ name }
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

		InputAxis(const Key& defKey, const std::string& name, float axisValue)
			: key{ defKey }
			, groupName{ name }
			, scaleValue{ axisValue }
			, axisValue{}
		{
		}

		Key key;
		std::string groupName;
		float scaleValue;
		float axisValue; // actual value when user has pressed the key
	};

	struct MouseState
	{
		UINT8 mouseKey; // use this to bitmask this frame pressed mouse key
		float x;
		float y;
	};

	struct GamepadState
	{
		float rightThumbX;
		float rightThumbY;
		float leftThumbX;
		float leftThumbY;
		float rightTrigger;
		float leftTrigger;
		UINT16 gamepadKey; // use this to bitmask this frame pressed keys
		UINT8 currentPlayerIndex;
		bool isActive;
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
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartCount{};
		static std::array<bool, 4> m_GamepadConnected;
		static constexpr UINT m_MaxSupportedController{ 4 };
		static constexpr float m_GamepadStateCheckTimeLimit{ 3.0f * 1000.0f };
		float m_ElapsedTime{};
		bool m_NextFramePassed{};

		bool InputActionExecuteCondition(InputAction& input, const MouseState& mouse, const UINT8* keyboard, const GamepadState* gamepad) const;
		bool InputAxisExecuteCondition(InputAxis& input, const MouseState& mouse, const UINT8* keyboard, const GamepadState* gamepad) const;
		bool IsMouseKeyPressed(MouseKey mouseKey, const MouseState& checkState, float* outAxisValue = nullptr) const;
		bool IsKeyboardKeyPressed(SDL_Keycode keycode, const UINT8* checkState) const;
		bool IsGamepadKeyPressed(GamePadKey keycode, const GamepadState* checkState, float* outAxisValue = nullptr) const;

		// Get Hardware state (use this once per frame)
		MouseState GetMouseState() const;
		const UINT8* GetKeyboardState() const;
		std::array<GamepadState, m_MaxSupportedController> GetGamepadState() const;
	};
}

