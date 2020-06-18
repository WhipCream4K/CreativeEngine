#include "pch.h"
#include "AudioTestScene.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
//#include "FontAsset.h"
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

	const auto daeLogoSprite{ Sprite::Create("./Resources/logo.png","DAELogo") };
	const auto background{ Sprite::Create("./Resources/Sprite/lmao.png","BigLmao") };

	m_Sprites.emplace_back(daeLogoSprite);
	m_Sprites.emplace_back(background);
	
	const auto& gotcha{ ResourceManager::Load<AudioClip>("./Resources/Sound/Gotcha.mp3","RickRoll") };

	const auto& daeLogo = CreateGameObject<GameObject>(glm::fvec3{ -300.0f,-300.0f,1.0f });
	daeLogo->GetTransform().SetScale(1.5f, 1.5f);
	auto spriteRenderer{ daeLogo->CreateComponent<SpriteRenderer>() };
	spriteRenderer->SetSprite(daeLogoSprite,true);

	const auto& backGroundObject = CreateGameObject<GameObject>(glm::fvec3{ 0.0f,0.0f,30.0f });
	//backGroundObject->GetTransform().SetScale(0.0f,0.0f);
	spriteRenderer = backGroundObject->CreateComponent<SpriteRenderer>();
	spriteRenderer->SetSprite(background,true);

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
