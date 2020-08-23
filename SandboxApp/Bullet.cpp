#include "pch.h"
#include "Bullet.h"
#include "Sprite.h"
#include "Animator.h"
#include "Movement.h"

void Bullet::SetLaunchDirection(const glm::fvec2& direction)
{
	m_LaunchDirection = direction;
}

void Bullet::Awake()
{
	using namespace dae;

	auto bulletTexture = ResourceManager::Load<DefaultTextureData>("./Resources/Digger/Bullet.tga", "Bullet");
	m_pBulletBeforeImpactSprites = Sprite::CreateSpriteSheet(bulletTexture, { 0.0f,0.0f }, { 32.0f,30.0f }, 3, 0.0f);
	m_pBulletAfterImpactSprites = Sprite::CreateSpriteSheet(bulletTexture, { 0.0f,30.0f }, { 32.0f,30.0f }, 3, 0.0f);

	auto spriteRenderer = CreateComponent<SpriteRenderer>();
	spriteRenderer->SetSprite(m_pBulletBeforeImpactSprites[0],true);
	
	auto bulletBeforeImpactClip = AnimationClip::Create(GetShared<Bullet>());
	bulletBeforeImpactClip->AddProperty(spriteRenderer, m_pBulletBeforeImpactSprites);
	bulletBeforeImpactClip->SetSampleRate(10);

	auto bulletAfterImpactClip = AnimationClip::Create(GetShared<Bullet>());
	bulletAfterImpactClip->AddProperty(spriteRenderer, m_pBulletAfterImpactSprites);
	bulletAfterImpactClip->SetSampleRate(10);

	m_pRefBulletAfterImpactClip = bulletAfterImpactClip;

	auto animator = CreateComponent<Animator>();

	animator->AddTransition(bulletBeforeImpactClip, "IsImpact", bulletAfterImpactClip,true);
	animator->SetDefaultAnimClip(bulletBeforeImpactClip);

	auto movement = CreateComponent<Movement>();
	movement->SetAcceleration(0.0f);
}

void Bullet::Update()
{
	if(!m_pRefBulletAfterImpactClip.expired())
	{
		if(m_pRefBulletAfterImpactClip.lock()->IsFinishedPlaying())
		{
			GetScene()->Destroy(GetShared<Bullet>());
			return;
		}
	}

	// Launch the bullet in the given direction
	if(m_pRefMovementComponent.expired())
	{
		auto movement = m_pRefMovementComponent.lock();
		movement->AddMovementInput(m_LaunchDirection, 1.0f);
	}
}

void Bullet::OnBeginOverlap(const std::vector<std::weak_ptr<dae::Collider>>& otherColliders)
{
	auto animator = m_pRefAnimator.lock();
	animator->SetTrigger("IsImpact");
	otherColliders;
}
