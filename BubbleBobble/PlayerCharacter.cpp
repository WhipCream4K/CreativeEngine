#include "pch.h"
#include "PlayerCharacter.h"
#include "Animator.h"
#include "SpriteRenderer.h"
#include "Texture2D.h"

void PlayerCharacter::Awake()
{
	using namespace dae;
	
	const auto& spriteRenderer= CreateComponent<SpriteRenderer>();
	const auto& animator = CreateComponent<Animator>();

	const auto characterTextureSrc{ Texture2D::Create("./Resources/PlayerCharacterSheet.tga","Player", SamplerState::SS_PixelSam) };
	const auto characterSprites{ Sprite::CreateMultiSprite(characterTextureSrc,{2.0f,35.0f},{16.0f,16.0f},4,2.0f) };

	const auto animationClip{ AnimationClip::Create(weak_from_this()) };
	spriteRenderer->SetSprite(characterSprites->sprites[0]);
	animationClip->AddProperty(spriteRenderer, characterSprites->sprites);
	animationClip->SetSampleRate(4);

	animator->AddAnimationClip(animationClip);
}
