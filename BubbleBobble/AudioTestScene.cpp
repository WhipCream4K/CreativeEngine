#include "pch.h"
#include "AudioTestScene.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "FontAsset.h"
#include "AudioSource.h"
#include "AudioClip.h"
#include "TextRenderer.h"

AudioTestScene::AudioTestScene()
	: Scene("AudioTestScene")
	, m_AudioPlayer{}
{
}

void AudioTestScene::SceneInitialize()
{
	using namespace dae;

	const auto daeLogoSprite{ Sprite::CreateSprite("./Resources/logo.png","DAELogo") };
	const auto background{ Sprite::CreateSprite("./Resources/background.jpg","BlackBG") };

	const auto& gotcha{ ResourceManager::Load<AudioClip>("./Resources/Sound/Gotcha.mp3","RickRoll") };
	const auto& font{ ResourceManager::Load<FontAsset>("./Resources/Lingua.otf","Lingua") };


	const auto& daeLogo = CreateGameObject<GameObject>(glm::fvec3{ -300.0f,-300.0f,1.0f });
	daeLogo->GetTransform().SetScale(1.5f, 1.5f);
	auto spriteRenderer{ daeLogo->CreateComponent<SpriteRenderer>() };
	spriteRenderer->SetSprite(daeLogoSprite);

	const auto& backGroundObject = CreateGameObject<GameObject>(glm::fvec3{ 0.0f,0.0f,30.0f });
	backGroundObject->GetTransform().SetScale(2.5f, 2.5f);
	spriteRenderer = backGroundObject->CreateComponent<SpriteRenderer>();
	spriteRenderer->SetSprite(background);

	const auto& text{ CreateGameObject({0.0f,0.0f,2.0f}) };
	auto textRenderer = text->CreateComponent<TextRenderer>();
	textRenderer->SetText("Congratulations! You've been rick rolled");
	textRenderer->SetFontAsset(font);
	textRenderer->SetSize(50);
	textRenderer->SetForegroundColor({ 1.0f,1.0f,1.0f,1.0f });

	const auto& rick{ CreateGameObject() };
	m_AudioPlayer = rick->CreateComponent<AudioSource>();
	m_AudioPlayer->SetAudioClip(gotcha);
}

void AudioTestScene::Update()
{
	if (!m_AudioPlayer->IsPlaying())
		m_AudioPlayer->Play();
}

void AudioTestScene::Render() const
{
}
