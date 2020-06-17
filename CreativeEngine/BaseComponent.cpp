#include "pch.h"
#include "BaseComponent.h"


dae::BaseComponent::BaseComponent()
	: m_pGameObject{}
	, m_IsActive{ true }
{
}

std::shared_ptr<dae::GameObject> dae::BaseComponent::GetGameObject() const
{
	return m_pGameObject.lock();
}

auto dae::BaseComponent::GetScene() const noexcept -> std::shared_ptr<Scene>
{
	return m_pGameObject.lock()->GetScene();
}
