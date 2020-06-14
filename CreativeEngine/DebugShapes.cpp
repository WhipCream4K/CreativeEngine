#include "pch.h"
#include "DebugShapes.h"

dae::DebugShapes::DebugShapes(const SDL_Color& color, float depth, bool isFilled)
	: m_Color{ color }
	, m_Depth{ depth }
	, m_IsFilled{ isFilled }
{
}
