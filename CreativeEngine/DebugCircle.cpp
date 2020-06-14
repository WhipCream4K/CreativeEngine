#include "pch.h"
#include "DebugCircle.h"
#include "Renderer.h"

dae::DebugCircle::DebugCircle(const glm::fvec3& center, float radius, const SDL_Color& color, float depth,
	bool isFilled)
	: DebugShapes(color, depth, isFilled)
	, m_Points{}
{
	const auto circleCenter{ EnginePointToSDLPoint(center) };

	const float deltaAngle{ XM_PI / 180.0f };
	for (float angle = 0; angle < XM_2PI; angle += deltaAngle)
	{
		SDL_Point point{ circleCenter.x + int(cosf(angle) * radius),circleCenter.y + int(sinf(angle) * radius) };
		m_Points.emplace_back(point);
	}
}

void dae::DebugCircle::Draw() const
{
	const auto& renderer{ Renderer::GetSDLRenderer().get() };
	SDL_SetRenderDrawColor(renderer, m_Color.r, m_Color.g, m_Color.b, m_Color.a);
	if (!m_IsFilled)
	{
		SDL_RenderDrawLines(renderer, &m_Points.front(), int(m_Points.size()));
	}
	else
	{
	}
}
