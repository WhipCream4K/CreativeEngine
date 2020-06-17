#pragma once
#include "BaseAsset.h"

namespace dae
{
	struct TextInfo
	{		
		std::string text;
		uint32_t ptSize;
		glm::fvec4 fgColor;
		glm::fvec4 bgColor;
	};
	
	class Texture2D;
	struct RenderTransform;
	class IFontEntity : public BaseAsset
	{
	public:

		IFontEntity(const std::string& assetPath, const std::string& assetName);
		virtual ~IFontEntity() = default;
		IFontEntity(const IFontEntity&) = delete;
		IFontEntity& operator=(const IFontEntity&) = delete;
		IFontEntity(IFontEntity&&) = delete;
		IFontEntity& operator=(IFontEntity&&) = delete;

		virtual void Render(const TextInfo& textInfo,const RenderTransform& transform,int renderFlags = 0) = 0;
	};
}

