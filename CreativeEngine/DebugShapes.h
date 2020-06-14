#pragma once

#include <SDL.h>


namespace dae
{
	class DebugShapes
	{
	public:

		DebugShapes(const SDL_Color& color, float depth, bool isFilled);
		virtual ~DebugShapes() = default;
		
		virtual void Draw() const = 0;
		float GetDepth() const { return m_Depth; }

	protected:
		
		SDL_Color m_Color;
		float m_Depth;
		bool m_IsFilled;
	};
}

