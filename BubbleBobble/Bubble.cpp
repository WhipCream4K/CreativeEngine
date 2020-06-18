#include "pch.h"
#include "Bubble.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "Animator.h"

Bubble::Bubble()
{
}

void Bubble::Awake()
{
	using namespace dae;

	const auto bubbleSprite{ ResourceManager::Load<DefaultTextureData>("./Resources/Sprite/Sprites1.png","Sprite1") };

	m_BubbleSpriteSheet = Sprite::CreateSpriteSheet(bubbleSprite, { 0.0f,192.0f }, { 16.0f,16.0f }, 8, 18.0f);
	auto animator = CreateComponent<Animator>();
	auto spriteRenderer = CreateComponent<SpriteRenderer>();

	auto shooting = AnimationClip::Create(GetShared<Bubble>());
	spriteRenderer->SetSprite(m_BubbleSpriteSheet[0],true);
	shooting->AddProperty(spriteRenderer, m_BubbleSpriteSheet);
	animator->AddAnimationClip(shooting);
}

void Bubble::Update()
{
}

void Bubble::Render() const
{
}
