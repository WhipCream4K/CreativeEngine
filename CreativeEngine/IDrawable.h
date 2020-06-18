#pragma once

namespace dae
{
	struct RenderTransform;
	class IDrawable
	{
	public:

		IDrawable() = default;
		virtual ~IDrawable() = default;

		virtual void Render(const TextureInfo&,const RenderTransform&) const = 0;
	};
}

