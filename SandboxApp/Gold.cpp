#include "pch.h"
#include "Gold.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "BoxCollider2D.h"
#include "PlayerDigger.h"
#include "PickUps.h"

void Gold::Awake()
{
	using namespace dae;
	auto moneybag = ResourceManager::Load<DefaultTextureData>("./Resources/Digger/MoneyBag.tga", "MoneyBag");
	auto moneybagDestroyed = ResourceManager::Load<DefaultTextureData>("./Resources/Digger/MoneyBag_Destroyed.tga", "MoneyBag_Destroyed");
	
	m_pMoneySprites = Sprite::CreateSpriteSheet(moneybag, {}, { 32.0f,30.0f }, 4, 0.0f);
	m_pMoneyDestroyedSprites = Sprite::CreateSpriteSheet(moneybagDestroyed, {}, { 32.0f,30.0f }, 3, 0.0f);

	auto spriteRenderer = CreateComponent<SpriteRenderer>();
	spriteRenderer->SetSprite(m_pMoneySprites[0],true);

	auto pickUps = CreateComponent<PickUps>();
	pickUps->SetGainScore(500);
	m_pRefPickUps = pickUps;
}

void Gold::Start()
{
	auto mainPlayer = GetScene()->GetGameObjectOfType<PlayerDigger>();
	if(mainPlayer)
	{
		m_pRefMainPlayer = mainPlayer;
	}
}

void Gold::Update()
{
}

void Gold::OnBeginOverlap(const std::vector<std::weak_ptr<dae::Collider>>& colliders)
{
	auto player = std::static_pointer_cast<GameObject>(m_pRefMainPlayer.lock());
	for (const auto& collider : colliders)
	{
		if(collider.lock()->GetGameObject() == player)
		{
			GetTransform()->SetRelativePosition({ 30.0f,0.0f,0.0f });
		}
	}
}
