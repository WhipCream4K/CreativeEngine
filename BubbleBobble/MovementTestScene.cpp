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

void MovementTestScene::SetUpInputMappingGroup()
{
	const auto& inputManager{ GetSceneContext().pInputManager };
	auto& actionGroup = inputManager->AddInputActionGroup("Jump");
	actionGroup.AddKey(dae::Key(dae::Device::D_Keyboard, SDLK_SPACE));
}

void MovementTestScene::Update()
{

}

void MovementTestScene::Render() const
{

}
