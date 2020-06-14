#include "pch.h"
#include "Debug.h"
#include "Renderer.h"
#include "DebugCircle.h"
#include "DebugRect.h"


void dae::Debug::RenderImpl()
{

#ifdef ASYNC

	//std::vector<std::future<void>> futures;

	////const auto& renderer{ Renderer::GetSDLRenderer().get() };
	//for (const auto& [rect,color,isFilled] : m_DebugRect)
	//{
	//	futures.push_back(std::async(std::launch::async, [](const SDL_Rect& rect, const SDL_Color& color, bool isFilled)
	//		{
	//			const auto& renderer{ Renderer::GetSDLRenderer().get() };
	//			SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	//			if (isFilled)
	//				SDL_RenderDrawRect(renderer, &rect);
	//			else
	//				SDL_RenderFillRect(renderer, &rect);
	//		}, rect, color, isFilled));
	//}

	//for (auto && future : futures)
	//{
	//	future.get();
	//}

#else

	SDL_SetRenderDrawBlendMode(Renderer::GetSDLRenderer().get(), SDL_BLENDMODE_BLEND);
	
	std::sort(m_pDebugObjects.begin(), m_pDebugObjects.end(), [](const std::shared_ptr<DebugShapes>& left, const std::shared_ptr<DebugShapes>& right)
		{
			return left->GetDepth() > right->GetDepth();
		});

	for (const auto& debugObject : m_pDebugObjects)
	{
		debugObject->Draw();
	}

	m_pDebugObjects.clear();

	//DrawEngineGrid();
	
#endif

}

void dae::Debug::DrawEngineGrid() const
{
	const auto engineSettings{ EngineContext::GetWindowSettings() };
	const auto& renderer{ Renderer::GetSDLRenderer().get() };

	const SDL_Point halfTop{ int(engineSettings.width / 2.0f),int(engineSettings.height) };
	const SDL_Point halfBottom{ int(engineSettings.width / 2.0f),0 };
	const SDL_Point halfLeft{0,int(engineSettings.height / 2.0f)};
	const SDL_Point halfRight{int(engineSettings.width),int(engineSettings.height / 2.0f)};

	const SDL_Color red{ Float4ToSDLColor({1.0f,0.0f,0.0f,0.3f }) };
	const SDL_Color green{ Float4ToSDLColor({0.0f,1.0f,0.0f,0.3f}) };

	SDL_SetRenderDrawColor(renderer, red.r, red.g, red.b, red.a);
	SDL_RenderDrawLine(renderer, halfLeft.x, halfLeft.y, halfRight.x, halfRight.y);
	SDL_SetRenderDrawColor(renderer, green.r, green.g, green.b, green.a);
	SDL_RenderDrawLine(renderer, halfTop.x, halfTop.y, halfBottom.x, halfBottom.y);

	// find how long(cm) for our current window
	const uint32_t halfWindowLength{ uint32_t(ceilf(float(engineSettings.width / 2.0f) * PixelToCent)) };
	const uint32_t halfWindowHeight{ uint32_t(ceilf(float(engineSettings.height / 2.0f) * PixelToCent)) };

	const SDL_Color grey{ 220,220,220,100 };
	SDL_SetRenderDrawColor(renderer, grey.r, grey.g, grey.b, grey.a);

	int pointX{int(float(engineSettings.width) / 2.0f)}; // Starts at the window center + 1 cm

	for (uint32_t x = 0; x < halfWindowLength; ++x)
	{
		pointX = int(float(pointX) - CentToPixel);
		SDL_Point top{ pointX,int(engineSettings.height) };
		SDL_Point bottom{ pointX,0 };
		SDL_RenderDrawLine(renderer, top.x, top.y, bottom.x, bottom.y);
	}
}

void dae::Debug::IDrawWireRect(const glm::fvec3& position, const glm::fvec2& dimension, const glm::fvec4& color)
{
	const auto rect{ EngineRectToSDLRect(position,dimension.x,dimension.y) };
	const auto sdlColor{ Float4ToSDLColor(color) };

	const auto debug{ std::make_shared<DebugRect>(rect,sdlColor,position.z,false) };

	m_pDebugObjects.emplace_back(debug);
}

void dae::Debug::IDrawWireRect(const glm::fvec3& position, float width, float height, const glm::fvec4& color)
{
	const auto rect{ EngineRectToSDLRect(position,width,height) };
	const auto sdlColor{ Float4ToSDLColor(color) };

	const auto debug{ std::make_shared<DebugRect>(rect,sdlColor,position.z,false) };

	m_pDebugObjects.emplace_back(debug);
}

void dae::Debug::IDrawRect(const glm::fvec3& position, const glm::fvec2& dimension, const glm::fvec4& color)
{
	const auto rect{ EngineRectToSDLRect(position,dimension.x,dimension.y) };
	const auto sdlColor{ Float4ToSDLColor(color) };

	const auto debug{ std::make_shared<DebugRect>(rect,sdlColor,position.z,true) };

	m_pDebugObjects.emplace_back(debug);
}

void dae::Debug::IDrawRect(const glm::fvec3& position, float width, float height, const glm::fvec4& color)
{
	const auto rect{ EngineRectToSDLRect(position,width,height) };
	const auto sdlColor{ Float4ToSDLColor(color) };

	const auto debug{ std::make_shared<DebugRect>(rect,sdlColor,position.z,true) };

	m_pDebugObjects.emplace_back(debug);
}

void dae::Debug::IDrawWireCircle(const glm::fvec3& position, float radius, const glm::fvec4& color)
{
	const auto sdlColor{ Float4ToSDLColor(color) };
	const auto debug{ std::make_shared<DebugCircle>(position,radius,sdlColor,position.z,false) };

	m_pDebugObjects.emplace_back(debug);
}
