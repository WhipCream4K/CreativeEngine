#pragma once
#include "Scene.h"

class MovementTestScene : public dae::Scene
{
public:
	
	MovementTestScene();

protected:
	void SceneInitialize() override;
	void SetUpInputMappingGroup() override;
	void Update() override;
	void Render() const override;
};

