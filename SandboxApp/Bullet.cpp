#include "pch.h"
#include "Bullet.h"
#include "Sprite.h"
#include "Animator.h"
#include "BoxCollider2D.h"
#include "Movement.h"
#include "Digger.h"
#include "Nobblin.h"

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
	spriteRenderer->SetSprite(m_pBulletBeforeImpactSprites[0], true);

	auto bulletBeforeImpactClip = AnimationClip::Create(GetShared<Bullet>());
	bulletBeforeImpactClip->AddProperty(spriteRenderer, m_pBulletBeforeImpactSprites);
	bulletBeforeImpactClip->SetSampleRate(10);

	auto bulletAfterImpactClip = AnimationClip::Create(GetShared<Bullet>());
	bulletAfterImpactClip->AddProperty(spriteRenderer, m_pBulletAfterImpactSprites);
	bulletAfterImpactClip->SetSampleRate(10);

	m_pRefBulletAfterImpactClip = bulletAfterImpactClip;

	auto animator = CreateComponent<Animator>();

	animator->AddTransition(bulletBeforeImpactClip, "IsImpact", bulletAfterImpactClip, true);
	animator->SetDefaultAnimClip(bulletBeforeImpactClip);

	m_pRefAnimator = animator;
	
	auto movement = CreateComponent<Movement>();
	movement->SetAcceleration(0.0f);
	movement->SetInitialVelocity(400.0f);
	m_pRefMovementComponent = movement;

	const auto& scale{ GetTransform()->GetScale() };
	auto collider2D = CreateComponent<BoxCollider2D>();
	collider2D->SetSize({ scale.x * 32.0f, scale.y * 30.0f });
	m_pRefMainScene = GetScene()->GetShared<Digger>();
}

void Bullet::Update()
{
	if (!m_IsImpact)
	{
		// check for collision with the play area
		const auto& currPos{ GetTransform()->GetPosition() };
		if(Digger::IsOutSidePlayArea(currPos))
			m_IsImpact = true;
		
		if (!m_pRefMainScene.expired())
		{
			const auto blockId = m_pRefMainScene.lock()->GetBlockIdFromWorldPos(GetTransform()->GetPosition());
			if (blockId == Digger::BlockId::Level)
				m_IsImpact = true;
		}

		if (m_IsImpact)
		{
			m_pRefAnimator.lock()->SetTrigger("IsImpact");
			return;
		}
		
		// Launch the bullet in the given direction
		if (!m_pRefMovementComponent.expired())
		{
			auto movement = m_pRefMovementComponent.lock();
			movement->AddMovementInput(m_LaunchDirection, 1.0f);
		}
		return;
	}


	if (!m_pRefBulletAfterImpactClip.expired())
	{
		if (m_pRefBulletAfterImpactClip.lock()->IsFinishedPlaying())
		{
			GetScene()->Destroy(GetShared<Bullet>());
		}
	}

}

void Bullet::OnBeginOverlap(const std::vector<std::weak_ptr<dae::Collider>>& otherColliders)
{
	//TODO: Add the tpye of object to cause bullet destruction
	for (const auto& collider : otherColliders)
	{
		const auto gameObject = collider.lock()->GetGameObject();
		if(std::dynamic_pointer_cast<Nobblin>(gameObject))
		{
			GetScene()->Destroy(gameObject);
		}
	}
	//auto animator = m_pRefAnimator.lock();
	//animator->SetTrigger("IsImpact");
	//otherColliders;
}
