#include "pch.h"
#include "Text.h"
#include "FontAsset.h"
#include "ResourceManager.h"

dae::Text::Text(std::weak_ptr<FontAsset> pFont, const std::string& text, uint32_t size, const glm::fvec4& FGColor,
	const glm::fvec4& BGColor)
{
	
}

dae::Text::Text(const std::string& assetPath, const std::string& fontName, const std::string& text, uint32_t size,
	const glm::fvec4& FGColor, const glm::fvec4& BGColor)
{
}

dae::RenderTexture dae::Text::GetRenderTexture()
{
}
