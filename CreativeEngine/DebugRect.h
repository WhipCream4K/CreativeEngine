#pragma once
#include "DebugShapes.h"

namespace dae
{
	class DebugRect final : public DebugShapes
	{
	public:
		DebugRect(const SDL_Rect& rect, const SDL_Color& color, float depth, bool isFilled);
		void Draw() const override;
	private:
		SDL_Rect m_Rect;
	};
}

