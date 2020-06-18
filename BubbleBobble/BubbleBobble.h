#pragma once
#include <Scene.h>

class BubbleBobble : public dae::Scene
{
public:

	BubbleBobble();

	void SceneInitialize() override;
	void Update() override;
	void Render() const override;
	void SetUpInputMappingGroup() override;

private:

	// container
	std::vector<std::shared_ptr<dae::Sprite>> m_pSprites;
};

