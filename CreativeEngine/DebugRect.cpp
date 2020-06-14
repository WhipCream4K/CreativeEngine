#include "pch.h"
#include "DebugRect.h"
#include "Renderer.h"

dae::DebugRect::DebugRect(const SDL_Rect& rect, const SDL_Color& color, float depth, bool isFilled)
	: DebugShapes(color, depth, isFilled)
	, m_Rect{ rect }
{
}

void dae::DebugRect::Draw() const
{
	const auto& renderer{ Renderer::GetSDLRenderer().get() };
	SDL_SetRenderDrawColor(renderer, m_Color.r, m_Color.g, m_Color.b, m_Color.a);
	if (m_IsFilled)
		SDL_RenderDrawRect(renderer, &m_Rect);
	else
		SDL_RenderFillRect(renderer, &m_Rect);
}
