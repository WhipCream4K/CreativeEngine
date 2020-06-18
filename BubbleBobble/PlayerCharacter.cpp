#include "pch.h"
#include "PlayerCharacter.h"
#include "Animator.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"

#include <iostream>

PlayerCharacter::PlayerCharacter()
	: m_pPlayerSpriteSheet{}
	, m_IsMoving{}
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

	const auto characterTextureSrc{ ResourceManager::Load<DefaultTextureData>("./Resources/PlayerCharacterSheet.tga","Player") };

	// Create sprite sheet
	{
		const uint32_t amount{ 4 };
		const glm::fvec2 dimension{ 16.0f,16.0f };
		glm::fvec2 minBound{ 2.0f,35.0f };
		const uint32_t pixelGap{ 2 };

		for (uint32_t i = 0; i < amount; ++i)
		{
			auto sprite{ Sprite::Create(characterTextureSrc) };

			sprite->SetDimension(i == 2 || i == 3 ? glm::fvec2{ 15.0f,16.0f } : dimension, true);
			sprite->SetSubTextureMinBounding(minBound);

			minBound.x += (i == 2 || i == 3 ? glm::fvec2{ 15.0f,16.0f } : dimension).x + pixelGap;

			m_pPlayerSpriteSheet.emplace_back(sprite);
		}
	}

	// Create Animator
	{
		const auto movingClip{ AnimationClip::Create(GetShared<PlayerCharacter>()) };
		const auto idle{ AnimationClip::Create(GetShared<PlayerCharacter>()) };

		idle->AddProperty(m_pSpriteRenderer, { m_pPlayerSpriteSheet[0] });

		const auto playerMovement = [](bool& isMoving)
		{
			return isMoving;
		};
		
		idle->AddTransition(AnimTransition({ std::make_shared<MulticastCondition<bool&>>(playerMovement,m_IsMoving) }, movingClip));
		movingClip->AddTransition(AnimTransition({ std::make_shared<MulticastCondition<bool&>>(std::not_fn(playerMovement),m_IsMoving) }, idle));
		
		m_pSpriteRenderer->SetSprite(m_pPlayerSpriteSheet[0],true);
		movingClip->AddProperty(m_pSpriteRenderer, m_pPlayerSpriteSheet);
		movingClip->SetSampleRate(4);

		// order matters
		animator->AddAnimationClip(idle);
		animator->AddAnimationClip(movingClip);
	}

}

void PlayerCharacter::SetUpInputComponent()
{
	if (!m_pInputComponent)
		m_pInputComponent = CreateComponent<dae::InputComponent>();

	// Setup input binding here
	m_pInputComponent->BindAction("Jump", dae::InputEvent::IE_Released, GetShared<PlayerCharacter>(), &PlayerCharacter::JumpTest);
	m_pInputComponent->BindAxis("Horizontal", GetShared<PlayerCharacter>(), &PlayerCharacter::MoveHorizontal);
	m_pInputComponent->BindAxis("Vertical", GetShared<PlayerCharacter>(), &PlayerCharacter::MoveVertical);
}

void PlayerCharacter::JumpTest()
{
	std::cout << "Yay finally it works\n";
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
