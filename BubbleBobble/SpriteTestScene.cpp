#include "pch.h"
#include "SpriteTestScene.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "DAELogo.h"
#include "PlayerCharacter.h"
#include "Sprite.h"
#include "Text.h"

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

	const auto daeLogoSprite{ Sprite::Create("./Resources/logo.png","DAELogo") };
	const auto background{ Sprite::Create("./Resources/background.jpg","BlackBG") };
	const auto debugRotation{ Sprite::Create("./Resources/Debug.png","Debug") };

	// TODO: Probably doing something like scene binding so the asset doesn't go out of scope
	m_pSprites.emplace_back(daeLogoSprite);
	m_pSprites.emplace_back(background);
	m_pSprites.emplace_back(debugRotation);

	const auto& defaultFont{ ResourceManager::Load<DefaultFontData>("./Resources/Font/Lingua.otf","Lingua") };

	const auto creativeText{ Text::Create(defaultFont,"CreativeEngine",50) };
	const auto transparentText{ Text::Create(defaultFont,"Transparent!",100) };

	m_pTextObjs.emplace_back(creativeText);
	m_pTextObjs.emplace_back(transparentText);

	const auto& daeLogo = CreateGameObject<GameObject>(glm::fvec3{ -300.0f,-300.0f,1.0f });
	daeLogo->GetTransform()->SetScale(1.5f, 1.5f);
	auto spriteRenderer{ daeLogo->CreateComponent<SpriteRenderer>() };
	spriteRenderer->SetSprite(daeLogoSprite,true);

	const auto& debugRuler = CreateGameObject<GameObject>({ 400.0f,0.0f,3.0f });
	spriteRenderer = debugRuler->CreateComponent<SpriteRenderer>();
	spriteRenderer->SetSprite(debugRotation,true);

	const auto& backGroundObject = CreateGameObject<GameObject>(glm::fvec3{ 0.0f,0.0f,30.0f });
	backGroundObject->GetTransform()->SetScale(2.5f, 2.5f);
	spriteRenderer = backGroundObject->CreateComponent<SpriteRenderer>();
	spriteRenderer->SetSprite(background,true);
	
	const auto& player = CreateGameObject<PlayerCharacter>({ 0.0f,-130.0f,2.0f });
	player->GetTransform()->SetScale(15.0f, 15.0f);

	m_pDAELogo = debugRuler;

	const auto& text = CreateGameObject<GameObject>(glm::fvec3{ 0.0f,0.0f,2.0f });
	m_pTextRenderer = text->CreateComponent<TextRenderer>();
	m_pTextRenderer->SetText(creativeText);

	const auto& text2 = CreateGameObject<GameObject>({ 300.0f,-300.0f,1.0f });
	const auto& textRenderer = text2->CreateComponent<TextRenderer>();
	textRenderer->SetText(transparentText);
	textRenderer->SetForegroundColor({ 1.0f,1.0f,1.0f,0.5f });
}

void SpriteTestScene::Update()
{
	const float deltaTime{ GetSceneContext().pGameTime->GetDeltaSeconds() };
	const float speed{ 2.0f };

	m_AnimTimer += deltaTime * speed;

	m_TextSize = abs(sinf(m_AnimTimer)) * 125.0f;
	m_TextSize = dae::Clamp(m_TextSize, 1.0f, 125.0f);
	m_pTextRenderer->SetTextSize(uint32_t(m_TextSize));

	if (m_AnimTimer >= dae::XM_2PI)
		m_AnimTimer -= dae::XM_2PI;

	const float rotateSpeed{ 45.0f };

	m_SpriteRotation += deltaTime * rotateSpeed;

	if (m_SpriteRotation >= 360.0f)
		m_SpriteRotation = 0.0f;

	m_pDAELogo->GetTransform()->SetRotation(0.0f, 0.0f, m_SpriteRotation);
}

void SpriteTestScene::Render() const
{
	dae::Debug::DrawRect({ 500.0f,0.0f,0.0f }, { 200.0f,200.0f });
	dae::Debug::DrawWireRect({ -500.0f,0.0f,0.0f }, { 200.0f,200.0f });
	dae::Debug::DrawWireCircle({ 0.0f,0.0f,1.0f }, 450.0f);
}
