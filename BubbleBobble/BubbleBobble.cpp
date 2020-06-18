#include "pch.h"
#include "BubbleBobble.h"
#include "PlayerCharacter.h"
#include "Sprite.h"
#include "SpriteRenderer.h"

BubbleBobble::BubbleBobble()
	: Scene("BubbleBobble")
{
}

void BubbleBobble::SceneInitialize()
{
	using namespace dae;
	const auto& player = CreateGameObject<PlayerCharacter>();
	player->GetTransform().SetScale(4.0f, 4.0f);

	const auto black{ Sprite::Create("./Resources/Sprite/Black.png","Black") };
	const auto background = CreateGameObject({0.0f,0.0f,30.0f});
	auto spriteRenderer = background->CreateComponent<SpriteRenderer>();
	spriteRenderer->SetSprite(black,true);
	background->GetTransform().SetScale(1600.0f, 900.0f);

	const auto levelSprite{ Sprite::Create("./Resources/Sprite/map1.png","Level1") };
	const auto level{ CreateGameObject({0.0f,0.0f,29.0f}) };
	spriteRenderer = level->CreateComponent<SpriteRenderer>();
	spriteRenderer->SetSprite(levelSprite,true);
	
	m_pSprites.emplace_back(black);
	m_pSprites.emplace_back(levelSprite);
}

void BubbleBobble::Update()
{
}

void BubbleBobble::Render() const
{
	
}

void BubbleBobble::SetUpInputMappingGroup()
{
	const auto& inputManager{ GetSceneContext().pInputManager };
	auto& actionGroup = inputManager->AddInputActionGroup("Jump");
	actionGroup.AddKey(dae::Key(dae::Device::D_Keyboard, SDLK_SPACE));
	auto& horizontal = inputManager->AddInputAxisGroup("Horizontal");
	horizontal.AddKey(dae::Key(dae::Device::D_Keyboard, SDLK_LEFT), -1.0f);
	horizontal.AddKey(dae::Key(dae::Device::D_Keyboard, SDLK_RIGHT), 1.0f);
	auto& vertical = inputManager->AddInputAxisGroup("Vertical");
	vertical.AddKey(dae::Key(dae::Device::D_Keyboard, SDLK_UP), 1.0f);
	vertical.AddKey(dae::Key(dae::Device::D_Keyboard, SDLK_DOWN), -1.0f);
}
