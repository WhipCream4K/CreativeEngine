#pragma once
#include <utility>
#include <vector>
#include <SDL.h>

#include "Singleton.h"

struct SDL_Window;
struct SDL_Renderer;
namespace dae
{
	class SpriteRenderer;
	class TextRenderer;
	class ITextureEntity;
	class IDrawable;

	//enum class SamplerState
//{
//	SS_Pixel,
//	SS_Linear,
//	SS_Anisotropic
//};

	struct RenderTexture
	{
		RenderTexture(std::weak_ptr<IDrawable>&& object, const TextureInfo& info, const RenderTransform& objTransform, float _depth)
			: transform(objTransform)
			, textureInfo{ info }
			, drawObject{ std::move(object) }
			, depth{ _depth }
		{
		}

		RenderTransform transform;
		TextureInfo textureInfo;
		std::weak_ptr<IDrawable> drawObject;
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
		//static void AssignRenderQueue(const RenderInfo& renderTexture, const glm::fvec2& pivot, const glm::fvec2& position, const glm::fvec2& scale, float rotation, float depth)
		//{
		//	GetInstance()->AssignRenderQueueImpl(renderTexture, pivot, position, scale, rotation, depth);
		//}

		//static void AssignRenderQueue(const RenderInfo& renderTexture, std::weak_ptr<ITextureEntity> textureEntity, float depth)
		//{
		//	GetInstance()->AssignRenderQueueImpl(renderTexture, std::move(textureEntity), depth);
		//}

		static void AssignRenderQueue(std::weak_ptr<IDrawable>&& drawObject, const RenderTransform& transform, float depth)
		{
			GetInstance()->AssignRenderQueueImpl(std::move(drawObject), transform, depth);
		}

		static void AssignRenderQueue(std::weak_ptr<IDrawable>&& drawObject, const TextureInfo& textureInfo, const RenderTransform& transform, float depth)
		{
			GetInstance()->AssignRenderQueueImpl(std::move(drawObject), textureInfo, transform, depth);
		}

		/**
		 * \brief
		 * This will render the render texture in front of everything at the given position and rotation
		 * \param renderTexture
		 * \param position
		 * \param dimension
		 * \param rotation
		 */
		static void RenderImmediate(const RenderInfo& renderTexture, const glm::fvec2& position, const glm::fvec2& dimension, float rotation)
		{
			GetInstance()->RenderImmediateImpl(renderTexture, position, dimension, rotation);
		}

	private:

		static void Render() { GetInstance()->RenderImpl(); }
		static void DebugRender() { GetInstance(); }
		static void Initialize(std::weak_ptr<SDL_Window> pWindow) { GetInstance()->InitializeImpl(std::move(pWindow)); }

		std::shared_ptr<SDL_Renderer> m_pRenderer;
		std::vector<RenderTexture> m_RenderTexture;

		void RenderImpl();
		void RenderImmediateImpl(const RenderInfo& renderTexture, const glm::fvec2& position, const glm::fvec2& dimension, float rotation);
		void InitializeImpl(std::weak_ptr<SDL_Window> pWindow);
		void AssignRenderQueueImpl(std::weak_ptr<IDrawable>&& drawObject, const RenderTransform& transform, float depth);
		void AssignRenderQueueImpl(std::weak_ptr<IDrawable>&& drawObject, const TextureInfo& textureInfo, const RenderTransform& transform, float depth);
	};

}

