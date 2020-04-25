#pragma once
#include "BaseComponent.h"

namespace dae
{
	class TextComponent : public BaseComponent
	{
	protected:
		void Render() override;
		void Start() override {}
		void Update() override {}
	};
}

