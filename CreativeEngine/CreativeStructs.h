#pragma once

#include <string>
#include <memory>

#include "glm.hpp"

namespace dae
{
	struct Window
	{
		Window() :
			width{ 640 },
			height{ 480 },
			title{ "Creative Engine by Patipan Poungjun 2DAE02" },
			aspectRatio{ float(width) / height }
		{
		}

		union
		{
			glm::uvec2 dimension;
			struct
			{
				uint32_t width, height;
			};
		};

		std::string title;
		float aspectRatio;
	};

	class GameTime;
	class InputManager;
	struct SceneContext
	{
		std::shared_ptr<GameTime> pGameTime;
		std::shared_ptr<InputManager> pInputManager;
	};
}
