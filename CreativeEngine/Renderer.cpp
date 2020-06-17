#include "pch.h"
#include "Renderer.h"
#include "ITextureEntity.h"
#include "IDrawable.h"

#include <algorithm>

void dae::Renderer::RenderImpl()
{
	if (m_RenderTexture.empty())
		return;

	std::sort(m_RenderTexture.begin(), m_RenderTexture.end(), [](const RenderTexture& left, const RenderTexture& right)
		{
			return left.depth > right.depth;
		});

	for (const auto& texture : m_RenderTexture)
	{
		texture.drawObject.lock()->Render(texture.transform);
		
		//if (SDL_RenderCopyEx(m_pRenderer.get(),
		//	surface.renderTexture.lock().get(),
		//	&surface.srcRect, &surface.surfaceRect,
		//	double(surface.rotation),
		//	&surface.pivot,
		//	SDL_FLIP_NONE))
		//	throw std::runtime_error(std::string("Renderer::Render() Failed to render texture ") + SDL_GetError());
	}

	m_RenderTexture.clear();
}

void dae::Renderer::RenderImmediateImpl(const RenderInfo& renderTexture, const glm::fvec2& position,
	const glm::fvec2& dimension, float rotation)
{
	renderTexture;
	position;
	dimension;
	rotation;
}

void dae::Renderer::InitializeImpl(std::weak_ptr<SDL_Window> pWindow)
{
	if (pWindow.lock())
		m_pRenderer = std::shared_ptr<SDL_Renderer>(
			SDL_CreateRenderer(pWindow.lock().get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
			SDLDeleter());

	if (!m_pRenderer)
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
}

//void dae::Renderer::AssignRenderQueueImpl(const RenderInfo& renderTexture, const glm::fvec2& pivot, const glm::fvec2& position,
//                                          const glm::fvec2& scale, float rotation, float depth)
//{
//
//	// the default pivot point of the sprite is at the top left of the texture
//	// but in this engine I'm going to change to the center of the texture instead
//	// |------------------------|
//	// |			|			|	
//	// |			|			|	
//	// |-----------	+ ----------|				
//	// |			|			|		
//	// |			|			|
//	// |------------------------|
//
//	//const auto totalTextureDimension{ renderTexture.GetDimension() * scale };
//	//const auto textureRect{ EngineRectToSDLRect(position, totalTextureDimension.x, totalTextureDimension.y) };
//
//	//m_RenderTexture renderSurface{};
//	//renderSurface.renderTexture = renderTexture.GetTexture();
//	//renderSurface.pivot = {int(pivot.x),int(pivot.y)};
//	//renderSurface.surfaceRect = textureRect;
//	//renderSurface.srcRect = { 0,0,int(totalTextureDimension.x),int(totalTextureDimension.y) };
//	////renderSurface.surfaceRect.x = int(sdlRenderOrigin.x);
//	////renderSurface.surfaceRect.y = int(sdlRenderOrigin.y);
//	////renderSurface.surfaceRect.w = int(totalTextureDimension.x);
//	////renderSurface.surfaceRect.h = int(totalTextureDimension.y);
//	//renderSurface.depth = depth;
//	//renderSurface.rotation = rotation;
//
//	//m_RenderTexture.emplace_back(renderSurface);
//}

void dae::Renderer::AssignRenderQueueImpl(const RenderInfo& renderTexture,
	std::weak_ptr<ITextureEntity> textureEntity, float depth)
{
	renderTexture;
	textureEntity;
	depth;
	//m_RenderTexture.emplace_back(m_RenderTexture(renderTexture,textureEntity,depth));
}

void dae::Renderer::AssignRenderQueueImpl(std::weak_ptr<IDrawable>&& drawObject, const RenderTransform& transform, float depth)
{
	RenderTexture renderTexture{ std::move(drawObject),transform,depth };
	m_RenderTexture.emplace_back(renderTexture);
}

void dae::Renderer::AssignRenderQueueImpl(std::shared_ptr<const IDrawable>&& drawObject, const RenderTransform& transform,
	float depth)
{
	drawObject;
	transform;
	depth;
	//RenderTexture renderTexture{ std::move(drawObject),transform,depth };
	//m_RenderTexture.emplace_back(renderTexture);
}
//void dae::Renderer::AssignRenderQueueImpl(const RenderInfo& renderTexture, const glm::fvec4& boundingBox,
//                                          const glm::fvec2& pivot, const glm::fvec2& position, const glm::fvec2& scale, float rotation, float depth)
//{
//	//const glm::fvec2 textureDimension{(boundingBox.z - boundingBox.x),(boundingBox.w - boundingBox.y)};
//	//const glm::fvec2 totalTextureDimension{ textureDimension * scale };
//	//const auto textureRect{ EngineRectToSDLRect(position,totalTextureDimension.x,totalTextureDimension.y) };
//	//
//	//m_RenderTexture renderSurface{};
//	//renderSurface.renderTexture = renderTexture.GetTexture();
//	//renderSurface.pivot = { int(pivot.x),int(pivot.y) };
//	//renderSurface.surfaceRect = textureRect;
//	//renderSurface.depth = depth;
//	//renderSurface.rotation = rotation;
//	//renderSurface.srcRect = { int(boundingBox.x),int(boundingBox.y),int(textureDimension.x),int(textureDimension.y) };
//
//	//m_RenderTexture.emplace_back(renderSurface);
//}
