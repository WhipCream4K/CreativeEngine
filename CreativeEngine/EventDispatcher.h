#pragma once
#include "BaseComponent.h"
#include "CreativeStructs.h"

namespace dae
{
	class DispatcherHandler final
	{
	public:

		void Broadcast() const;

		template<typename UserObject = dae::GameObject, typename ...Args>
		constexpr auto RegisterObserver(
			std::weak_ptr<dae::GameObjectType<UserObject>>&& observer,
			dae::Event<dae::GameObjectType<UserObject>, Args...> func,
			Args... arguments) -> void;

		template<typename UserObject = dae::GameObject>
		constexpr auto RegisterObserver(std::weak_ptr<dae::GameObjectType<UserObject>>&& observer,
			dae::Event<dae::GameObjectType<UserObject>> func) -> void;

	private:

		std::string m_Name;
		std::vector<std::shared_ptr<dae::IMuticastAction>> m_pActions;
	};

	template <typename UserObject, typename ... Args>
	constexpr auto DispatcherHandler::RegisterObserver(std::weak_ptr<dae::GameObjectType<UserObject>>&& observer,
		dae::Event<dae::GameObjectType<UserObject>, Args...> func, Args... arguments) -> void
	{
		auto delegateWithParams{ std::make_shared<dae::DelegateWithParams<UserObject,Args...>>(std::move(func),std::move(observer),arguments) };
		m_pActions.emplace_back(delegateWithParams);
	}

	template <typename UserObject>
	constexpr auto DispatcherHandler::RegisterObserver(std::weak_ptr<dae::GameObjectType<UserObject>>&& observer,
		dae::Event<dae::GameObjectType<UserObject>> func) -> void
	{
		auto delegateNoParam{ std::make_shared<dae::DelegateNoParam<UserObject>>(std::move(func),std::move(observer)) };
		m_pActions.emplace_back(delegateNoParam);
	}

	class EventDispatcher final : public dae::BaseComponent
	{
	public:

		DispatcherHandler* CreateDispatcher(const std::string& name);

	private:

		std::unordered_map<std::string, DispatcherHandler> m_Dispatchers;
	};
}


