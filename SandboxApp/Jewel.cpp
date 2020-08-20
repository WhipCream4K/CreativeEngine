#include "pch.h"
#include "Jewel.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Sprite.h"
#include "PickUps.h"

void Jewel::Awake()
{
	using namespace dae;
	auto jewel = ResourceManager::Load<DefaultTextureData>("./Resources/Digger/Jewel.tga", "Jewel");
	m_pJewelSprite = Sprite::Create(jewel);

	auto spriteRenderer = CreateComponent<SpriteRenderer>();
	spriteRenderer->SetSprite(m_pJewelSprite,true);

	auto pickUps = CreateComponent<PickUps>();
}

void Jewel::Update()
{
	
}
