#include "pch.h"
#include "MainGame.h"
#include "SpriteTestScene.h"
#include "SceneManager.h"
#include "AudioTestScene.h"
#include "MovementTestScene.h"
#include "BubbleBobble.h"

dae::MainGame::MainGame()
	: CreativeCore()
{
}

void dae::MainGame::GameInitialize()
{
	//SceneManager::AddScene(std::make_shared<MovementTestScene>());
	//SceneManager::AddScene(std::make_shared<AudioTestScene>());
	//SceneManager::AddScene(std::make_shared<SpriteTestScene>());
	SceneManager::AddScene(std::make_shared<BubbleBobble>());
}
