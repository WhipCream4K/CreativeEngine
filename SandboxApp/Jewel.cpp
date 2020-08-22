#include "pch.h"
#include "Jewel.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Sprite.h"
#include "PickUps.h"
#include "PlayerDigger.h"

void Jewel::Awake()
{
	using namespace dae;
	auto jewel = ResourceManager::Load<DefaultTextureData>("./Resources/Digger/Jewel.tga", "Jewel");
	m_pJewelSprite = Sprite::Create(jewel);

	auto spriteRenderer = CreateComponent<SpriteRenderer>();
	spriteRenderer->SetSprite(m_pJewelSprite,true);

	auto pickUps = CreateComponent<PickUps>();
	pickUps->SetGainScore(25);
	m_pRefPickUps = pickUps;
}

void Jewel::Start()
{
	auto mainPlayer = GetScene()->GetGameObjectOfType<PlayerDigger>();
	m_pRefPlayer = mainPlayer;
}

void Jewel::Update()
{
	
}

void Jewel::OnBeginOverlap(const std::vector<std::weak_ptr<dae::Collider>>& others)
{
	auto playerObject = std::static_pointer_cast<GameObject>(m_pRefPlayer.lock());
	for (const auto& collider : others)
	{
		if (collider.lock()->GetGameObject() == playerObject)
		{
			PickUps::OnScoreUpdate.Broadcast(m_pRefPickUps.lock()->GetGainScore());
			GetScene()->Destroy(GetShared<Jewel>());
		}
	}
}
