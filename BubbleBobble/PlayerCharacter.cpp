#include "pch.h"
#include "PlayerCharacter.h"
#include "Animator.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"

#include <iostream>

PlayerCharacter::PlayerCharacter()
{
	// set default param
	using namespace dae;

	const auto& spriteRenderer = CreateComponent<SpriteRenderer>();
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

	const auto animationClip{ AnimationClip::Create(weak_from_this()) };
	spriteRenderer->SetSprite(m_pPlayerSpriteSheet[0]);
	animationClip->AddProperty(spriteRenderer, m_pPlayerSpriteSheet);
	animationClip->SetSampleRate(4);

	animator->AddAnimationClip(animationClip);
}

void PlayerCharacter::Awake()
{
	SetUpInputComponent();
}

void PlayerCharacter::SetUpInputComponent()
{
	// Setup input binding here
	m_pInputComponent->BindAction("Jump", dae::InputEvent::IE_Released, GetShared<PlayerCharacter>(), &PlayerCharacter::JumpTest);
}

void PlayerCharacter::JumpTest()
{
	std::cout << "Yay finally it works\n";
}
