#pragma once
#include "GameObject.h"

class TestGameObject : public dae::GameObject, public std::enable_shared_from_this<TestGameObject>
{
public:
	void Spawn();
	void Start() override;
};

