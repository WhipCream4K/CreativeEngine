#include "pch.h"
#include "SpriteTestScene.h"
#include "GameObject.h"
#include "SpriteRenderer.h"

SpriteTestScene::SpriteTestScene()
	: Scene("SpriteTestScene")
{
}

void SpriteTestScene::SceneInitialize()
{
	using namespace dae;
	
	const auto deaLogo = std::make_shared<GameObject>();
	const Sprite deaSprite{ "./Resources/logo.png","deaLogo",SpriteMode::SM_Single };
	deaLogo->CreateComponent<SpriteRenderer>(deaSprite);

	AddGameObject(deaLogo);
}

void SpriteTestScene::SetUpInputAction()
{
}
