#pragma once
#include <type_traits>
#include <glm.hpp>

#include "EngineContext.h"

namespace dae
{
	constexpr float XM_PI{ 3.141592654f };
	constexpr float XM_PIDIV2{ 1.570796327f };
	constexpr float XM_2PI{ 6.283185307f };
	constexpr float PixelToCent{ 0.0264583333f };
	constexpr float CentToPixel{ 37.7952755906f };

	template<typename T>
	inline typename std::enable_if<std::is_arithmetic_v<T>, T>::type Clamp(T value,T low,T hi)
	{
		//assert(!(hi < low));
		return (value < low) ? low : (hi < value) ? hi : value;
	}

	inline SDL_Rect EngineRectToSDLRect(const glm::fvec3& objectPos, float textureWidth, float textureHeight)
	{
		const auto& engineSettings{ EngineContext::GetWindowSettings() };

		return SDL_Rect{
			int(engineSettings.width / 2.0f - textureWidth / 2.0f + objectPos.x),
			int(engineSettings.height - engineSettings.height / 2.0f - textureHeight / 2.0f - objectPos.y),
			int(textureWidth),
			int(textureHeight)
		};
	}

	inline SDL_Rect EngineRectToSDLRect(const glm::fvec2& objectPos, float textureWidth, float textureHeight)
	{
		const auto& engineSettings{ EngineContext::GetWindowSettings() };

		return SDL_Rect{
			int(engineSettings.width / 2.0f - textureWidth / 2.0f + objectPos.x),
			int(engineSettings.height - engineSettings.height / 2.0f - textureHeight / 2.0f - objectPos.y),
			int(textureWidth),
			int(textureHeight)
		};
	}

	inline SDL_Point EnginePointToSDLPoint(float x, float y)
	{
		const auto& engineSettings{ EngineContext::GetWindowSettings() };

		return SDL_Point{
			int(engineSettings.width / 2.0f + x),
			int(engineSettings.height - engineSettings.height / 2.0f - y)
		};
	}

	inline SDL_Point EnginePointToSDLPoint(const glm::fvec2& position)
	{
		const auto& engineSettings{ EngineContext::GetWindowSettings() };

		return SDL_Point{
			int(engineSettings.width / 2.0f + position.x),
			int(engineSettings.height - engineSettings.height / 2.0f - position.y)
		};
	}

	// RegisterProvider the pivot point of the GameObject in context of this engine then give you the SDL pivot point
	inline SDL_Point EnginePivotPointToSDLPoint(const glm::fvec2& pivot,float textureWidth,float textureHeight)
	{
		return SDL_Point{
			int(textureWidth / 2.0f + pivot.x),
			int(textureHeight - textureHeight / 2.0f - pivot.y)
		};
	}

	// RegisterProvider the pivot point of the GameObject in context of this engine then give you the SDL pivot point
	inline SDL_Point EnginePivotPointToSDLPoint(float x, float y, float textureWidth, float textureHeight)
	{
		return SDL_Point{
			int(textureWidth / 2.0f + x),
			int(textureHeight - textureHeight / 2.0f - y)
		};
	}

	inline SDL_Point EnginePivotPointToSDLPoint(const glm::fvec2& pivot,const glm::fvec2& textureDimension)
	{
		return SDL_Point{
			int(textureDimension.x / 2.0f + pivot.x),
			int(textureDimension.y - textureDimension.y / 2.0f - pivot.y)
		};
	}

	inline glm::fvec4 Saturate(const glm::fvec4& color)
	{
		return glm::fvec4{
			Clamp(color.r,0.0f,1.0f),
			Clamp(color.g,0.0f,1.0f),
			Clamp(color.b,0.0f,1.0f),
			Clamp(color.a,0.0f,1.0f)
		};
	}
	
	
	inline SDL_Color Float4ToSDLColor(const glm::fvec4& color)
	{
		const glm::fvec4 satureColor{ Saturate(color) };
		
		return SDL_Color{
			Uint8(satureColor.r * 255.0f),
			Uint8(satureColor.g * 255.0f),
			Uint8(satureColor.b * 255.0f),
			Uint8(satureColor.a * 255.0f)
		};
	}

	inline SDL_Point Float2ToSDLPoint(const glm::fvec2& point)
	{
		return SDL_Point{ int(point.x),int(point.y) };
	}

	inline SDL_Rect Float4ToSDLRect(const glm::fvec4& rect)
	{
		return SDL_Rect{ int(rect.x),int(rect.y),int(rect.z),int(rect.w) };
	}

}

