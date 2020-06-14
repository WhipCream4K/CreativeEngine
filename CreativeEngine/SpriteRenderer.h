#pragma once
#include "BaseComponent.h"

struct SDL_Texture;
namespace dae
{
	class Sprite;
	class Renderer;
	class SpriteRenderer final : public BaseComponent
	{
	public:

		//void SetSprite(const Sprite& sprite);
		void SetSprite(std::weak_ptr<Sprite> sprite);
		//const Sprite& GetSprite() const { return m_Sprite; }
		std::shared_ptr<Sprite> GetSprite() const { return m_pSprite.lock(); }
		void SetPivotPoint(const glm::fvec2& pivot);
		
	protected:
		void Render() const override;
	private:
		//Sprite m_Sprite;
		std::weak_ptr<Sprite> m_pSprite;
	};

}


