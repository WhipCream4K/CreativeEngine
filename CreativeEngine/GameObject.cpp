#include "pch.h"
#include "GameObject.h"
#include "BaseComponent.h"
#include "Transform.h"

dae::GameObject::GameObject()
	: m_pTransform()
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
	m_pTransform = std::make_shared<Transform>();
	m_pComponents.emplace_back(m_pTransform);
	
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
