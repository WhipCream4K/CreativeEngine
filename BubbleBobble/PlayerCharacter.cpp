#include "pch.h"
#include "PlayerCharacter.h"
#include "Animator.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "Bubble.h"

#include <iostream>


PlayerCharacter::PlayerCharacter()
	: m_pPlayerSpriteSheet{}
	, m_IsMoving{}
	, m_IsShooting{}
{
	SetMaxSpeed(200.0f);
}

void PlayerCharacter::Awake()
{
	SetUpInputComponent();

	// set default param
	using namespace dae;

	m_pSpriteRenderer = CreateComponent<SpriteRenderer>();
	const auto& animator = CreateComponent<Animator>();

	const auto characterTextureSrc{ ResourceManager::Load<DefaultTextureData>("./Resources/Sprite/Sprites0.png","Sprite0") };
	const auto characterTextureSrc2{ ResourceManager::Load<DefaultTextureData>("./Resources/Sprite/Sprites1.png","Sprite1") };

	m_pPlayerSpriteSheet = Sprite::CreateSpriteSheet(characterTextureSrc, { 0.0f,0.0f }, { 16.0f,16.0f }, 8, 18.0f);
	m_pShootingSpriteSheet = Sprite::CreateSpriteSheet(characterTextureSrc2, { 0.0f,64.0f }, { 16.0f,16.0f }, 8, 18.0f);

	//// Create sprite sheet
	//{
	//	const uint32_t amount{ 4 };
	//	const glm::fvec2 dimension{ 16.0f,16.0f };
	//	glm::fvec2 minBound{ 2.0f,35.0f };
	//	const uint32_t pixelGap{ 2 };

	//	for (uint32_t i = 0; i < amount; ++i)
	//	{
	//		auto sprite{ Sprite::Create(characterTextureSrc) };

	//		sprite->SetDimension(i == 2 || i == 3 ? glm::fvec2{ 15.0f,16.0f } : dimension, true);
	//		sprite->SetSubTextureMinBounding(minBound);

	//		minBound.x += (i == 2 || i == 3 ? glm::fvec2{ 15.0f,16.0f } : dimension).x + pixelGap;

	//		m_pPlayerSpriteSheet.emplace_back(sprite);
	//	}
	//}

	// Create Animator
	{
		const auto movingClip{ AnimationClip::Create(GetShared<PlayerCharacter>()) };
		const auto idle{ AnimationClip::Create(GetShared<PlayerCharacter>()) };
		const auto shooting{ AnimationClip::Create(GetShared<PlayerCharacter>()) };

		idle->AddProperty(m_pSpriteRenderer, { m_pPlayerSpriteSheet[0] });

		const auto playerMovement = [](bool& isMoving)
		{
			return isMoving;
		};

		idle->AddTransition(AnimTransition({ std::make_shared<MulticastCondition<bool&>>(playerMovement,m_IsMoving) }, movingClip));
		idle->AddTransition(AnimTransition({ std::make_shared<MulticastCondition<bool&>>(playerMovement,m_IsShooting) }, shooting));
		movingClip->AddTransition(AnimTransition({ std::make_shared<MulticastCondition<bool&>>(std::not_fn(playerMovement),m_IsMoving) }, idle));
		movingClip->AddTransition(AnimTransition({ std::make_shared<MulticastCondition<bool&>>(playerMovement,m_IsShooting) }, shooting));
		shooting->AddTransition(AnimTransition({ std::make_shared<MulticastCondition<bool&>>(std::not_fn(playerMovement),m_IsShooting) }, idle));
		shooting->AddTransition(AnimTransition({ std::make_shared<MulticastCondition<bool&>>(std::not_fn(playerMovement),m_IsShooting) }, movingClip));

		m_pSpriteRenderer->SetSprite(m_pPlayerSpriteSheet[0], true);
		movingClip->AddProperty(m_pSpriteRenderer, m_pPlayerSpriteSheet);
		movingClip->SetSampleRate(8);

		shooting->AddProperty(m_pSpriteRenderer, m_pShootingSpriteSheet);
		shooting->SetSampleRate(8);

		animator->AddAnimationClip(idle);
		animator->AddAnimationClip(movingClip);
		animator->AddAnimationClip(shooting);
		animator->SetDefaultAnimClip(idle);
	}

}

void PlayerCharacter::SetUpInputComponent()
{
	if (!m_pInputComponent)
		m_pInputComponent = CreateComponent<dae::InputComponent>();

	// Setup input binding here
	m_pInputComponent->BindAction("Jump", dae::InputEvent::IE_Released, GetShared<PlayerCharacter>(), &PlayerCharacter::JumpTest);
	m_pInputComponent->BindAction("Reset", dae::InputEvent::IE_Released, GetShared<PlayerCharacter>(), &PlayerCharacter::Reset);
	m_pInputComponent->BindAxis("Horizontal", GetShared<PlayerCharacter>(), &PlayerCharacter::MoveHorizontal);
	m_pInputComponent->BindAxis("Vertical", GetShared<PlayerCharacter>(), &PlayerCharacter::MoveVertical);
}

void PlayerCharacter::JumpTest()
{
	GetScene()->Instantiate<Bubble>(GetTransform().GetPosition(),{},{4.0f,4.0f});
	m_IsShooting = true;
}

void PlayerCharacter::Reset()
{
	m_IsShooting = false;
}

void PlayerCharacter::MoveVertical(float value)
{
	const glm::fvec3 worldUp{ 0.0f,1.0f,0.0f };
	AddMovementInput(worldUp, value);
	m_IsMoving = true;
}

void PlayerCharacter::MoveHorizontal(float value)
{
	const glm::fvec3 worldRight{ 1.0f,0.0f,0.0f };
	AddMovementInput(worldRight, value);
	m_IsMoving = true;

	m_pSpriteRenderer->SetFlipY(value < 1.0f ? true : false);
}

void PlayerCharacter::LateUpdate()
{
	m_IsMoving = false;
}
