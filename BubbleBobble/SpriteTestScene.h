#pragma once
#include "Scene.h"

class SpriteTestScene :
	public dae::Scene
{
public:
	SpriteTestScene();

protected:
	void SceneInitialize() override;
	void SetUpInputAction() override;
};

