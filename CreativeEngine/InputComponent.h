#pragma once
#include "BaseComponent.h"
#include "InputManager.h"

namespace dae
{
	template<typename ...Args>
	constexpr bool none_void_check = (!std::is_void_v<Args> && ...);

	template<typename ...Args>
	using all_none_void = std::enable_if_t<(none_void_check<Args...>)>;

	struct IAxisMulticast;
	struct ActionHandlerSignature
	{
		template<typename T>
		using ObjectFunctionDelegate = void(T::*)();
		template<typename T, typename ...Args>
		using ObjectVariadicDelegate = void(T::*)(all_none_void<Args...>);
	};

	struct AxisHandlerSignature
	{
		template<typename T>
		using ObjectFunctionDelegate = void(T::*)(float);
		template<typename T, typename ...Args>
		using ObjectVariadicDelegate = void(T::*)(float, std::enable_if_t<(sizeof...(Args) > 0), size_t>);
		using AxisMulticastDelegate = std::shared_ptr<IAxisMulticast>;
	};

	template<typename T, typename ...Args>
	using VariadicFuncPtr = void(T::*)(Args...);
	template<typename T>
	using FuncPtr = void(T::*)();

	struct ActionHandle
	{
		ActionHandle(const std::string& aActionName, InputEvent event, ActionFunc&& func)
			: action(std::move(func))
			, actionName{ aActionName }
			, inputEvent{ event }
			, isTriggered{}
			, isPressedFlag{}
		{
		}

		void ShouldExecute();

		ActionFunc action;
		std::string actionName;
		InputEvent inputEvent;
		bool isTriggered;
		// use to check whether last frame, this button was pressed
		bool isPressedFlag;
	};

	struct AxisHandle
	{
		AxisHandle(const std::string& aAxisName, AxisHandlerSignature::AxisMulticastDelegate&& func)
			: action(std::move(func))
			, axisName{ aAxisName }
			, totalAxisValue{}
			, isTriggered{}
		{
		}

		void ShouldExecute(float axisValue);

		AxisHandlerSignature::AxisMulticastDelegate action;
		std::string axisName;
		float totalAxisValue;
		bool isTriggered;
	};

	template<typename UserClass, typename ...Args>
	struct DelegateCallbackWithParams : IMuticastAction
	{
		DelegateCallbackWithParams(void(UserClass::* funcName)(Args...), std::weak_ptr<UserClass>&& callObject, Args&&... args)
			: funcPtr(funcName)
			, caller(std::move(callObject))
			, arguments{ std::move(args) }
		{
		}

		void Invoke() override;

		VariadicFuncPtr<UserClass, Args...> funcPtr;
		std::weak_ptr<UserClass> caller;
		std::tuple<Args...> arguments;
	};

	template<typename UserClass>
	struct DelegateCallbackNoParams : IMuticastAction
	{
		DelegateCallbackNoParams(void(UserClass::* funcName)(), std::weak_ptr<UserClass>&& callObject)
			: funcPtr(funcName)
			, caller{ std::move(callObject) }
		{
		}

		void Invoke() override;

		FuncPtr<UserClass> funcPtr;
		std::weak_ptr<UserClass> caller;
	};

	template <typename UserClass, typename ... Args>
	void DelegateCallbackWithParams<UserClass, Args...>::Invoke()
	{
		auto temp{ caller.lock() };
		if (temp)
			((*temp).*funcPtr)(arguments...);
	}

	template <typename UserClass>
	void DelegateCallbackNoParams<UserClass>::Invoke()
	{
		auto temp{ caller.lock() };
		if (temp)
			((*temp).*funcPtr)();
	}

	struct IAxisMulticast
	{
		virtual ~IAxisMulticast() = default;
		virtual void Invoke(float axisValue) = 0;
	};

	template<typename UserClass, typename ...Args>
	struct InputAxisWithParamsCallback : IAxisMulticast
	{
		InputAxisWithParamsCallback(void(UserClass::* funcName)(float, Args...), std::weak_ptr<UserClass>&& callObject, Args&&... args)
			: funcPtr{ funcName }
			, caller{ std::move(callObject) }
			, arguments{ std::move(args...) }
		{
		}

		void Invoke(float axisValue) override;

		VariadicFuncPtr<UserClass, float, Args...> funcPtr;
		std::weak_ptr<UserClass> caller;
		std::tuple<Args...> arguments;
	};

	template<typename UserClass>
	struct InputAxisNoParamsCallback : IAxisMulticast
	{
		InputAxisNoParamsCallback(void(UserClass::* funcName)(float), std::weak_ptr<UserClass>&& callObject)
			: funcPtr{ funcName }
			, caller{ std::move(callObject) }
		{
		}

		void Invoke(float axisValue) override;

		VariadicFuncPtr<UserClass,float> funcPtr;
		std::weak_ptr<UserClass> caller;
	};

	template <typename UserClass, typename ... Args>
	void InputAxisWithParamsCallback<UserClass, Args...>::Invoke(float axisValue)
	{
		auto temp{ caller.lock() };
		if (temp)
			((*temp).*funcPtr)(axisValue, arguments...);
	}

	template <typename UserClass>
	void InputAxisNoParamsCallback<UserClass>::Invoke(float axisValue)
	{
		auto temp{ caller.lock() };
		if (temp)
			((*temp).*funcPtr)(axisValue);
	}

	class InputComponent final : public BaseComponent
	{
	public:

		void Awake() override;

		// Action mapping
		template<typename UserClass>
		constexpr auto BindAction(const std::string& actionName, InputEvent inputEvent,
			std::weak_ptr<GameObjectType<UserClass>>&& caller,
			ActionHandlerSignature::ObjectFunctionDelegate<UserClass> funcPtr) -> void;

		template<typename UserClass, typename ...Args>
		constexpr auto BindAction(const std::string& actionName, InputEvent inputEvent,
			std::weak_ptr<GameObjectType<UserClass>>&& caller,
			ActionHandlerSignature::ObjectVariadicDelegate<UserClass, Args...> funcPtr,
			Args&&... args) -> void;

		// Axis mapping
		template<typename UserClass>
		constexpr auto BindAxis(const std::string& axisName,
			std::weak_ptr<GameObjectType<UserClass>>&& caller,
			AxisHandlerSignature::ObjectFunctionDelegate<UserClass> funcPtr) -> void;

		template<typename UserClass, typename ...Args>
		constexpr auto BindAxis(const std::string& axisName,
			std::weak_ptr<GameObjectType<UserClass>>&& caller,
			AxisHandlerSignature::ObjectVariadicDelegate<UserClass, Args...> funcPtr,
			Args&&... args) -> void;

		constexpr auto GetActionHandles()  noexcept ->  std::unordered_map<std::string, ActionHandle>& { return m_ActionHandle; }
		constexpr auto GetAxisHandles()  noexcept ->  std::unordered_map<std::string, AxisHandle>& { return m_AxisHandle; }


	protected:

		void Update() override;

	private:

		std::unordered_map<std::string, ActionHandle> m_ActionHandle;
		std::unordered_map<std::string, AxisHandle> m_AxisHandle;

	};

	template <typename UserClass>
	constexpr auto InputComponent::BindAction(const std::string& actionName, InputEvent inputEvent,
		std::weak_ptr<GameObjectType<UserClass>>&& caller,
		ActionHandlerSignature::ObjectFunctionDelegate<UserClass> funcPtr) -> void
	{
		auto callback{ std::make_shared<DelegateCallbackNoParams<UserClass>>(funcPtr,std::move(caller)) };
		ActionHandle handle{ actionName,inputEvent,std::move(callback) };
		m_ActionHandle.try_emplace(actionName, std::move(handle));
	}

	template <typename UserClass, typename ... Args>
	constexpr auto InputComponent::BindAction(const std::string& actionName, InputEvent inputEvent,
		std::weak_ptr<GameObjectType<UserClass>>&& caller,
		ActionHandlerSignature::ObjectVariadicDelegate<UserClass, Args...> funcPtr, Args&&... args) -> void
	{
		auto callback{ std::make_shared<DelegateCallbackWithParams<UserClass,Args...>>(funcPtr,std::move(caller),std::move(args...)) };
		ActionHandle handle{ actionName,inputEvent,std::move(callback) };
		m_ActionHandle.try_emplace(actionName, std::move(handle));
	}

	template <typename UserClass>
	constexpr auto InputComponent::BindAxis(const std::string& axisName,
		std::weak_ptr<GameObjectType<UserClass>>&& caller,
		AxisHandlerSignature::ObjectFunctionDelegate<UserClass> funcPtr) -> void
	{
		auto callback{ std::make_shared<InputAxisNoParamsCallback<UserClass>>(funcPtr,std::move(caller)) };
		AxisHandle handle{ axisName,std::move(callback) };
		m_AxisHandle.try_emplace(axisName, std::move(handle));
	}

	template <typename UserClass, typename ... Args>
	constexpr auto InputComponent::BindAxis(const std::string& axisName,
		std::weak_ptr<GameObjectType<UserClass>>&& caller,
		AxisHandlerSignature::ObjectVariadicDelegate<UserClass, Args...> funcPtr, Args&&... args) -> void
	{
		auto callback{ std::make_shared<InputAxisWithParamsCallback<UserClass,Args...>>(funcPtr,std::move(caller),std::move(args...)) };
		AxisHandle handle{ axisName,std::move(callback) };
		m_AxisHandle.try_emplace(axisName, std::move(handle));
	}
}

