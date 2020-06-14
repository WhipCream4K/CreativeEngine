#pragma once
#include "Singleton.h"
#include <future>


namespace dae
{
	class DebugShapes;
	class Debug : public Singleton<Debug>
	{
		
		// TODO: Try to get rid of this dependencies
		friend class SceneManager;

	public:

		static void DrawWireRect([[maybe_unused]]const glm::fvec3& position, 
			[[maybe_unused]] const glm::fvec2& dimension, 
			[[maybe_unused]] const glm::fvec4& color = { 0.0f, 1.0f, 0.0f, 1.0f })
		{
#ifdef _DEBUG
			GetInstance()->IDrawWireRect(position, dimension,color);
#endif
		}
		static void DrawWireRect([[maybe_unused]] const glm::fvec3& position, 
			[[maybe_unused]] float width, [[maybe_unused]] float height, 
			[[maybe_unused]] const glm::fvec4& color = { 0.0f, 1.0f, 0.0f, 1.0f })
		{
#ifdef _DEBUG
			GetInstance()->IDrawWireRect(position, width, height,color);
#endif
		}

		static void DrawRect([[maybe_unused]] const glm::fvec3& position, 
			[[maybe_unused]] float width, [[maybe_unused]] float height, 
			[[maybe_unused]] const glm::fvec4& color = { 0.0f, 1.0f, 0.0f, 1.0f })
		{
#ifdef _DEBUG
			GetInstance()->IDrawRect(position, width, height, color);
#endif
		}

		static void DrawRect([[maybe_unused]] const glm::fvec3& position, 
			[[maybe_unused]] const glm::fvec2& dimension, 
			[[maybe_unused]] const glm::fvec4& color = { 0.0f, 1.0f, 0.0f, 1.0f })
		{
#ifdef _DEBUG
			GetInstance()->IDrawRect(position, dimension, color);
#endif
		}

		static void DrawWireCircle([[maybe_unused]] const glm::fvec3& position,
			[[maybe_unused]] float radius,
			[[maybe_unused]] const glm::fvec4& color = {0.0f,1.0f,0.0f,1.0f})
		{
#ifdef _DEBUG
			GetInstance()->IDrawWireCircle(position, radius, color);
#endif
		}
		
	private:

		static void Render()
		{
#ifdef _DEBUG
			GetInstance()->RenderImpl();
#endif
		}
		
		void RenderImpl();
		void DrawEngineGrid() const;
		void IDrawWireRect(const glm::fvec3& position, const glm::fvec2& dimension, const glm::fvec4& color = { 0.0f, 1.0f, 0.0f, 1.0f });
		void IDrawWireRect(const glm::fvec3& position, float width, float height,const glm::fvec4& color = { 0.0f, 1.0f, 0.0f, 1.0f });
		void IDrawRect(const glm::fvec3& position, const glm::fvec2& dimension, const glm::fvec4& color = { 0.0f, 1.0f, 0.0f, 1.0f });
		void IDrawRect(const glm::fvec3& position, float width, float height, const glm::fvec4& color = { 0.0f, 1.0f, 0.0f, 1.0f });
		void IDrawWireCircle(const glm::fvec3& position, float radius, const glm::fvec4& color);
		
		//std::vector<DebugRect> m_DebugRect;
		//std::vector<DebugCircle> m_DebugCircle;
		std::vector<std::shared_ptr<DebugShapes>> m_pDebugObjects;
		std::vector<SDL_Point> m_GridCoordinates;
	};
}



