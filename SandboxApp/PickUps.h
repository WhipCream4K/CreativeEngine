#pragma once
#include <BaseComponent.h>

class PickUps : public dae::BaseComponent
{
protected:
	void Awake() override;
	void Update() override;
};

