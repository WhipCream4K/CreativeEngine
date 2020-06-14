#pragma once
#include "BaseComponent.h"
#include "InputManager.h"
#include <functional>

namespace dae
{
	//template<typename T>
	//using ActionFunc = void(T::*)();
	
	class InputComponent : public BaseComponent
	{
	public:

		// TODO: Please finish this
		//template<typename UserClass>
		//void BindAction(const std::string& keyName, InputEvent inputEvent, std::weak_ptr<UserClass> pGameObject, ActionFunc<UserClass> func);

		//template<class UserClass>
		//void BindAction(const std::string& keyName, InputEvent inputEvent, UserClass* pGameObject, ActionFunc<UserClass> func);
	
	protected:
		void Update() override;
	};

	//template <typename UserClass>
	//void InputComponent::BindAction(const std::string& keyName, InputEvent inputEvent,
	//	std::weak_ptr<UserClass> pGameObject, ActionFunc<UserClass> func)
	//{
	//}

	//template <class UserClass>
	//void InputComponent::BindAction(const std::string& keyName, InputEvent inputEvent, UserClass* pGameObject,
	//	ActionFunc<UserClass> func)
	//{
	//}
}

