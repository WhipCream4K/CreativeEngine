#pragma once
#include "Singleton.h"

struct SDL_Window;
struct SDL_Renderer;
namespace dae
{
	class Renderer : public Singleton<Renderer>
	{
	public:

		~Renderer();
		
		void Initialize(SDL_Window* pWindow);
		void Destroy();
		SDL_Renderer* GetSDLRenderer();
	private:
		SDL_Renderer* m_pRenderer;
	};

}

