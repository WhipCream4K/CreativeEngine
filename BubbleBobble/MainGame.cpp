#include "pch.h"
#include "MainGame.h"
#include "SpriteTestScene.h"
#include "SceneManager.h"
#include "AudioTestScene.h"

dae::MainGame::MainGame()
	: CreativeCore()
{
}

void dae::MainGame::GameInitialize()
{
	SceneManager::AddScene(std::make_shared<AudioTestScene>());
}
