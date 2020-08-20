#include "pch.h"
#include "EventDispatcher.h"

void dae::DispatcherHandler::Broadcast() const
{
	for (const auto action : m_pActions)
	{
		action->Invoke();
	}
}

dae::DispatcherHandler* dae::EventDispatcher::CreateDispatcher(const std::string& name)
{
	auto dispatcher{ DispatcherHandler() };
	auto it = m_Dispatchers.try_emplace(name, dispatcher);
	if (it.second)
		return &m_Dispatchers.at(name);
	return nullptr;
}
