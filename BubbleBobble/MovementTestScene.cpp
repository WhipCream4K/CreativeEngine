#include "pch.h"
#include "MovementTestScene.h"
#include "PlayerCharacter.h"

MovementTestScene::MovementTestScene()
	: Scene("MovementTestScene")
{
}

void MovementTestScene::SceneInitialize()
{
	const auto playerCharacter{ CreateGameObject<PlayerCharacter>() };
	playerCharacter->GetTransform().SetScale(15.0f, 15.0f);
}

void MovementTestScene::Update()
{
	
}

void MovementTestScene::Render() const
{
	
}
