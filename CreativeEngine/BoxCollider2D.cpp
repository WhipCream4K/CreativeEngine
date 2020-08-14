#include "pch.h"
#include "BoxCollider2D.h"

void dae::BoxCollider2D::Update()
{
	const auto& ownerTransform{ GetGameObject()->GetTransform() };
	m_Center = ownerTransform->GetPosition();
}

void dae::BoxCollider2D::Render() const
{
	if(IsEnableDebugDraw())
	{
		
	}
}
