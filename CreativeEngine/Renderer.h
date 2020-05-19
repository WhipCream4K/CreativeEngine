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

		static SDL_Renderer* GetSDLRenderer() { return GetInstance().m_pRenderer; }
		static void Initialize(SDL_Window* pWindow) { GetInstance().InitializeImpl(pWindow); }
		static void Destroy() { GetInstance().DestroyImpl(); }

	private:
		SDL_Renderer* m_pRenderer;

		void InitializeImpl(SDL_Window* pWindow);
		void DestroyImpl();
	};

}

