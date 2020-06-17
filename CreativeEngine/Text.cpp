#include "pch.h"
#include "Text.h"
#include "IFontEntity.h"
#include "ResourceManager.h"

dae::Text::Text()
	: m_pFontAsset{}
	, m_FGColor{ 1.0f,1.0f,1.0f,1.0f }
	, m_BGColor{}
	, m_Text{}
	, m_Size{ 15 }
{
}

dae::Text::Text(std::weak_ptr<IFontEntity>&& fontAsset)
	: m_pFontAsset(std::move(fontAsset))
	, m_FGColor{ 1.0f,1.0f,1.0f,1.0f }
	, m_BGColor{}
	, m_Text{}
	, m_Size{ 15 }
{
}

auto dae::Text::Create(const std::string& fontPath, const std::string& fontName) -> std::shared_ptr<Text>
{
	const auto& font{ ResourceManager::Load<DefaultFontData>(fontPath,fontName) };
	auto text{ std::make_shared<Text>(font) };
	return text;
}

auto dae::Text::Create(std::weak_ptr<IFontEntity>&& fontEntity, const std::string& text,
	uint32_t size) -> std::shared_ptr<Text>
{
	auto textObj{ std::make_shared<Text>(fontEntity.lock()) };
	textObj->SetText(text);
	textObj->SetTextSize(size);
	return textObj;
}

//dae::Text dae::Text::Create(const std::string& fontPath, const std::string& fontName)
//{
//	const auto& font{ ResourceManager::Load<DefaultFontData>(fontPath,fontName) };
//	auto textObj{ Text(font) };
//	return textObj;
//}

void dae::Text::Render([[maybe_unused]]const TextureInfo& textureInfo, const RenderTransform& transform) const
{
	// create render info for font to render
	TextInfo info{};
	info.text = m_Text;
	info.fgColor = m_FGColor;
	info.bgColor = m_BGColor;
	info.ptSize = m_Size;

	m_pFontAsset.lock()->Render(info, transform);
}
