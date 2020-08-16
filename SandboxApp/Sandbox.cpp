#include "pch.h"
#include "Sandbox.h"
#include "SceneManager.h"
#include "Digger.h"

void Sandbox::GameInitialize()
{
	dae::SceneManager::AddScene(std::make_shared<Digger>());
}
