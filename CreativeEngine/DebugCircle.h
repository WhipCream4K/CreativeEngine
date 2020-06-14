#pragma once
#include "DebugShapes.h"
#include <vector>

namespace dae
{
	class DebugCircle final : public DebugShapes
	{
	public:
		DebugCircle(const glm::fvec3& center,float radius, const SDL_Color& color, float depth, bool isFilled);
		void Draw() const override;
	private:
		std::vector<SDL_Point> m_Points;
	};

}

