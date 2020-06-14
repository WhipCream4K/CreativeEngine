#pragma once
#include <vector>
#include <SDL.h>

#include "Singleton.h"

struct SDL_Window;
struct SDL_Renderer;
namespace dae
{
	class SpriteRenderer;
	class TextRenderer;

	struct RenderSurface
	{
		std::weak_ptr<SDL_Texture> renderTexture;
		SDL_Rect surfaceRect;
		SDL_Rect srcRect; // for when the entire texture doesn't get render
		SDL_Point pivot;
		float rotation;
		float depth;
	};
	
	class Renderer final : public Singleton<Renderer>
	{

		// TODO: Try to get rid of this dependencies
		friend class SceneManager;
		friend class CreativeCore;

	public:

		static std::shared_ptr<SDL_Renderer> GetSDLRenderer() { return GetInstance()->m_pRenderer; }

		// Get all the render component in the scene, sort them out by the order
		// then render all the target with that order
		static void AssignRenderQueue(const RenderTexture& renderTexture, const glm::fvec2& pivot,const glm::fvec2& position,const glm::fvec2& scale, float rotation, float depth)
		{
			GetInstance()->AssignRenderQueueImpl(renderTexture, pivot,position, scale,rotation, depth);
		}

		static void AssignRenderQueue(const RenderTexture& renderTexture, const glm::fvec4& boundingBox,const glm::fvec2& pivot,const glm::fvec2& position,
		const glm::fvec2& scale,float rotation,float depth)
		{
			GetInstance()->AssignRenderQueueImpl(renderTexture, boundingBox,pivot, position, scale, rotation, depth);
		}
		
		/**
		 * \brief
		 * This will render the render texture in front of everything at the given position and rotation
		 * \param renderTexture 
		 * \param position 
		 * \param dimension 
		 * \param rotation 
		 */
		static void RenderImmediate(const RenderTexture& renderTexture, const glm::fvec2& position, const glm::fvec2& dimension, float rotation)
		{
			GetInstance()->RenderImmediateImpl(renderTexture, position, dimension, rotation);
		}

		~Renderer();
		
	private:

		static void Render() { GetInstance()->RenderImpl(); }
		static void DebugRender() { GetInstance(); }
		static void Initialize(std::weak_ptr<SDL_Window> pWindow) { GetInstance()->InitializeImpl(pWindow); }

		std::shared_ptr<SDL_Renderer> m_pRenderer;
		std::vector<RenderSurface> m_RenderSurfaces;

		void RenderImpl();
		void RenderImmediateImpl(const RenderTexture& renderTexture, const glm::fvec2& position, const glm::fvec2& dimension, float rotation);
		void InitializeImpl(std::weak_ptr<SDL_Window> pWindow);
		void AssignRenderQueueImpl(const RenderTexture& renderTexture, const glm::fvec2& pivot, const glm::fvec2& position, const glm::fvec2& scale, float rotation, float depth);
		void AssignRenderQueueImpl(const RenderTexture& renderTexture, const glm::fvec4& boundingBox, const glm::fvec2& pivot,const glm::fvec2& position,
			const glm::fvec2& scale, float rotation, float depth);
	};

}

