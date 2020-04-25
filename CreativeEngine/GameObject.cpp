#include "pch.h"
#include "GameObject.h"
#include "InputComponent.h"

dae::GameObject::GameObject()
	: m_IsStatic{}
	, m_IsActive{ true }
	, m_Transform{}
	, m_Tag{}
{
}

void dae::GameObject::Render() const
{
	for (auto& component : m_pComponents)
	{
		component->Render();
	}
}
