#include "pch.h"
#include "GameObject.h"
#include "BaseComponent.h"
#include "Transform.h"
#include "PhysicsComponent.h"

dae::GameObject::GameObject()
	: m_pTransform()
	, m_Tag{}
	, m_IsStatic{}
	, m_IsActive{ true }
	, m_IsInit{}
{
	auto transform = std::make_shared<Transform>();
	m_pComponents.emplace_back(transform);
	m_pTransform = transform;
}

void dae::GameObject::RootRender() const
{
	Render();

	for (const auto& component : m_pComponents)
	{
		component->Render();
	}

	//for (const auto& physicsComponent : m_pPhysicsComponent)
	//{
	//	physicsComponent->Render();
	//}
}

void dae::GameObject::RootAwake()
{	
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

void dae::GameObject::RegisterCollider(const std::shared_ptr<BaseComponent>& component)
{
	GetScene()->GetPhysicsScene()->RegisterCollider(std::static_pointer_cast<Collider>(component));
}
