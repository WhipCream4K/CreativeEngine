#include "pch.h"
#include "MainGame.h"
#include "SpriteTestScene.h"
#include "SceneManager.h"

dae::MainGame::MainGame()
	: CreativeCore()
{
	const auto pSpriteTestScene{ std::shared_ptr<SpriteTestScene>() };
	SceneManager::GetInstance().AddScene(pSpriteTestScene);
}
