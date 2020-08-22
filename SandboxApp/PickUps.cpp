#include "pch.h"
#include "PickUps.h"
#include "BoxCollider2D.h"

dae::EventDispatcher<int> PickUps::OnScoreUpdate{};

PickUps::PickUps()
	: m_pCollider()
	, m_GainScore()
{
}

void PickUps::SetBoundSize(const glm::fvec2& size) const
{
	m_pCollider.lock()->SetSize(size);
}

void PickUps::Awake()
{
	using namespace dae;
	auto collider = GetGameObject()->CreateComponent<BoxCollider2D>();
	m_pCollider = collider;
	collider->SetSize({ 36.0f,36.0f });
}

void PickUps::Update()
{
}
