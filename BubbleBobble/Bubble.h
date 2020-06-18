#pragma once
#include <GameObject.h>


class Bubble : public dae::GameObject
{
public:

	Bubble();

protected:

	void Awake() override;
	void Update() override;
	void Render() const override;

private:
	
	// container
	std::vector<std::shared_ptr<dae::Sprite>> m_BubbleSpriteSheet;
};

