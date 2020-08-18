#include "pch.h"
#include "PhysicsComponent.h"

void dae::PhysicsComponent::RegisterOwner(std::weak_ptr<GameObject>&& owner)
{
	m_pRefSceneObject = owner.lock()->GetScene();
	m_pRefGameObject = std::move(owner);
}
