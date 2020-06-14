#pragma once
#include "GameObject.h"

class DAELogo : public dae::GameObject
{
public:
	
	DAELogo();

protected:
	void Start() override;
	void Update() override;
};

