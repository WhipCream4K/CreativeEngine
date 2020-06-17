#pragma once
#include "Scene.h"

namespace dae
{
	class TextRenderer;
	class SpriteRenderer;
	class Text;
}

class SpriteTestScene : public dae::Scene
{
public:

	SpriteTestScene();

protected:
	
	void SceneInitialize() override;
	void Update() override;
	void Render() const override;

private:

	float m_TextSize;
	float m_AnimTimer;
	float m_SpriteRotation;
	std::shared_ptr<dae::TextRenderer> m_pTextRenderer;
	std::shared_ptr<dae::GameObject> m_pDAELogo;

	// Container
	std::vector<std::shared_ptr<dae::Text>> m_pTextObjs;
	std::vector<std::shared_ptr<dae::Sprite>> m_pSprites;
};

