#include "pch.h"
#include "SpriteTestScene.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "ResourceManager.h"
#include "FontAsset.h"
#include "DAELogo.h"
#include "PlayerCharacter.h"
#include "Sprite.h"
#include "Texture2D.h"
#include "PlayerCharacter.h"

SpriteTestScene::SpriteTestScene()
	: Scene("SpriteTestScene")
	, m_TextSize{}
	, m_AnimTimer{}
	, m_SpriteRotation{}
{
}

void SpriteTestScene::SceneInitialize()
{
	using namespace dae;

	//const Sprite logoSprite{ "./Resources/logo.png","DAELogo",SpriteMode::SM_Single };
	//const Sprite backGround{ "./Resources/background.jpg","BlackBG",SpriteMode::SM_Single };
	//const Sprite debugRotation{ "./Resources/Debug.png","Debug",SpriteMode::SM_Single };

	//const auto playerSpriteSheet{ Texture2D::Create("./Resources/PlayerCharacterSheet.tga","Player",SamplerState::SS_PixelSam) };
	
	const auto daeLogoSprite{ Sprite::CreateSprite("./Resources/logo.png","DAELogo") };
	const auto background{ Sprite::CreateSprite("./Resources/background.jpg","BlackBG") };
	const auto debugRotation{ Sprite::CreateSprite("./Resources/Debug.png","Debug") };
	//const auto playerSprite{ Sprite::CreateMultiSprite(playerSpriteSheet,
	//	{2.0f,35.0f},{16.0f,16.0f},4,2.0f) };

	const auto& font{ ResourceManager::Load<FontAsset>("./Resources/Lingua.otf","Lingua") };

	const auto& daeLogo = CreateGameObject<GameObject>(glm::fvec3{ -300.0f,-300.0f,1.0f });
	daeLogo->GetTransform().SetScale(1.5f, 1.5f);
	auto spriteRenderer{ daeLogo->CreateComponent<SpriteRenderer>() };
	spriteRenderer->SetSprite(daeLogoSprite);

	const auto& debugRuler = CreateGameObject<GameObject>({ 400.0f,0.0f,3.0f });
	spriteRenderer = debugRuler->CreateComponent<SpriteRenderer>();
	spriteRenderer->SetSprite(debugRotation);

	const auto& backGroundObject = CreateGameObject<GameObject>(glm::fvec3{ 0.0f,0.0f,30.0f });
	backGroundObject->GetTransform().SetScale(2.5f, 2.5f);
	spriteRenderer = backGroundObject->CreateComponent<SpriteRenderer>();
	spriteRenderer->SetSprite(background);

	const auto& player = CreateGameObject<PlayerCharacter>({0.0f,-130.0f,2.0f});
	player->GetTransform().SetScale(15.0f, 15.0f);
	//
	//const auto& player = CreateGameObject({});
	//spriteRenderer = player->CreateComponent<SpriteRenderer>();
	//player->GetTransform().SetScale(15.0f, 15.0f);
	//spriteRenderer->SetSprite(playerSprite->sprites[3]);

	m_pDAELogo = debugRuler;

	const auto& text = CreateGameObject<GameObject>(glm::fvec3{ 0.0f,0.0f,2.0f });
	m_pTextRenderer = text->CreateComponent<TextRenderer>();
	m_pTextRenderer->SetFontAsset(font);
	m_pTextRenderer->SetSize(50);
	m_pTextRenderer->SetText("Creative Engine");
	m_pTextRenderer->SetForegroundColor({ 1.0f,1.0f,1.0f,1.0f });

	const auto& transparentText = CreateGameObject<GameObject>({ 300.0f,-300.0f,1.0f });
	const auto& textRenderer = transparentText->CreateComponent<TextRenderer>();
	textRenderer->SetFontAsset(font);
	textRenderer->SetForegroundColor({ 1.0f,1.0f,1.0f,0.5f });
	textRenderer->SetSize(100);
	textRenderer->SetText("Transparent!");
}

void SpriteTestScene::Update()
{
	const float deltaTime{ GetSceneContext().pGameTime->GetDeltaSeconds() };
	const float speed{ 2.0f };

	m_AnimTimer += deltaTime * speed;

	m_TextSize = abs(sinf(m_AnimTimer)) * 125.0f;
	m_TextSize = dae::Clamp(m_TextSize, 1.0f, 125.0f);
	m_pTextRenderer->SetSize(uint32_t(m_TextSize));

	if (m_AnimTimer >= dae::XM_2PI)
		m_AnimTimer -= dae::XM_2PI;

	const float rotateSpeed{ 45.0f };

	m_SpriteRotation += deltaTime * rotateSpeed;

	if (m_SpriteRotation >= 360.0f)
		m_SpriteRotation = 0.0f;

	m_pDAELogo->GetTransform().SetRotation(0.0f, 0.0f, m_SpriteRotation);
}

void SpriteTestScene::Render() const
{
	dae::Debug::DrawRect({ 500.0f,0.0f,0.0f }, { 200.0f,200.0f });
	dae::Debug::DrawWireRect({ -500.0f,0.0f,0.0f }, { 200.0f,200.0f });
	dae::Debug::DrawWireCircle({ 0.0f,0.0f,1.0f }, 450.0f);
}
