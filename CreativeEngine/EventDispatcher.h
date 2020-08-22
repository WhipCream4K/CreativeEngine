#pragma once
#include "BaseComponent.h"
#include "CreativeStructs.h"

namespace dae
{
	template<typename UserObject, typename ...Args>
	struct DispatcherDelegate : IMuticastAction
	{
		DispatcherDelegate(void(UserObject::* funcptr)(Args...), std::weak_ptr<UserObject>&& caller, std::tuple<Args...>* params)
			: pFuncptr(std::move(funcptr))
			, pCaller(std::move(caller))
			, pArgs(params)
		{
		}

		void Invoke() override;

		Event<UserObject, Args...> pFuncptr;
		std::weak_ptr<UserObject> pCaller;
		std::tuple<Args...>* pArgs;
	};

	template <typename UserObject, typename ... Args>
	void DispatcherDelegate<UserObject, Args...>::Invoke()
	{
		auto temp{ pCaller.lock() };
		if (temp)
			std::apply(pFuncptr, std::tuple_cat(std::make_tuple(temp), *pArgs));
	}

	template<typename ...Args>
	class DispatcherHandler final
	{
	public:

		constexpr auto Broadcast(Args... arguments) -> void;

		template<typename UserObject = dae::GameObject>
		constexpr auto RegisterObserver(
			std::weak_ptr<UserObject>&& observer,
			dae::Event<UserObject, Args...> func) -> void;

	private:

		std::tuple<Args...> m_Params;
		std::vector<std::shared_ptr<IMuticastAction>> m_pObserver;
	};

	template <typename ... Args>
	constexpr auto DispatcherHandler<Args...>::Broadcast(Args... arguments) -> void
	{
		m_Params = std::forward_as_tuple(arguments...);
		for (const auto& observer : m_pObserver)
			observer->Invoke();
	}

	template <typename ... Args>
	template <typename UserObject>
	constexpr auto DispatcherHandler<Args...>::RegisterObserver(std::weak_ptr<UserObject>&& observer,
		dae::Event<UserObject, Args...> func) -> void
	{
		auto action = std::make_shared<DispatcherDelegate<UserObject, Args...>>(std::move(func), std::move(observer), &m_Params);
		m_pObserver.emplace_back(action);
	}

	
	template<typename ...Types>
	using EventDispatcher = DispatcherHandler<Types...>;
	//class EventDispatcher final
	//{
	//public:

	//	template<typename ...Args>
	//	static constexpr std::shared_ptr<DispatcherHandler<Args...>> CreateDispatcher();

	//};

	//template <typename ... Args>
	//constexpr std::shared_ptr<DispatcherHandler<Args...>> EventDispatcher::CreateDispatcher()
	//{
	//	std::shared_ptr<DispatcherHandler<Args...>> handler{ std::make_shared<DispatcherHandler<Args...>>() };
	//	return handler;
	//}
} 


