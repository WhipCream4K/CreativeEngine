#include "pch.h"
#include "GameObject.h"

dae::GameObject::GameObject()
	: m_Transform{}
	, m_Tag{}
	, m_IsStatic{}
	, m_IsActive{ true }
	, m_IsInit{}
{
}

void dae::GameObject::RootRender() const
{
	Render();

	for (const auto& component : m_pComponents)
	{
		component->Render();
	}
}

void dae::GameObject::RootAwake()
{
	static_cast<IInternalComponent*>(&m_Transform)->RegisterOwner(weak_from_this());
	Awake();

	for (const auto& component : m_pComponents)
	{
		component->Awake();
	}
}

void dae::GameObject::RootStart()
{
	Start();

	for (const auto& component : m_pComponents)
	{
		component->Start();
	}
}

void dae::GameObject::RootUpdate()
{
	Update();

	for (const auto& component : m_pComponents)
	{
		component->Update();
	}
}

void dae::GameObject::RootLateUpdate()
{
	LateUpdate();

	for (const auto& component : m_pComponents)
	{
		component->LateUpdate();
	}
}
