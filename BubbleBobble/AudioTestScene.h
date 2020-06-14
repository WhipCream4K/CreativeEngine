#pragma once
#include "Scene.h"

namespace dae
{
	class AudioSource;
}

class AudioTestScene : public dae::Scene
{
public:
	AudioTestScene();
protected:
	void SceneInitialize() override;
	void Update() override;
	void Render() const override;

private:

	std::shared_ptr<dae::AudioSource> m_AudioPlayer;
};

