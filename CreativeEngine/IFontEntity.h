#pragma once
#include "BaseAsset.h"

namespace dae
{
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

