#pragma once

#include <string>
#include <memory>
#include <functional>
#include <type_traits>

#include <glm.hpp>
#include <SDL.h>
#include <SDL_ttf.h>
#include <fmod.hpp>

struct SDL_Texture;
namespace dae
{
	struct Window
	{
		Window() :
			width{ 1280 },
			height{ 720 },
			title{ "Creative Engine by Patipan Poungjun 2DAE02" },
			aspectRatio{ float(width) / height }
		{
		}

		union
		{
			glm::uvec2 dimension;
			struct
			{
				uint32_t width, height;
			};
		};

		std::string title;
		float aspectRatio;
	};

	class GameTime;
	class InputManager;
	struct SceneContext
	{
		std::shared_ptr<GameTime> pGameTime;
		std::shared_ptr<InputManager> pInputManager;
	};

	struct SDLDeleter
	{
		void operator()(TTF_Font* pFont) const
		{
			if (pFont)
			{
				TTF_CloseFont(pFont);
				pFont = nullptr;
			}
		}

		void operator()(SDL_Surface* pSurface) const
		{
			if (pSurface)
			{
				SDL_FreeSurface(pSurface);
				pSurface = nullptr;
			}
		}

		void operator()(SDL_Texture* pTexture) const
		{
			if (pTexture)
			{
				SDL_DestroyTexture(pTexture);
				pTexture = nullptr;
			}
		}

		void operator()(SDL_Renderer* pRenderer) const
		{
			if (pRenderer)
			{
				SDL_DestroyRenderer(pRenderer);
				pRenderer = nullptr;
			}
		}

		void operator()(SDL_Window* pWindow) const
		{
			if (pWindow)
			{
				SDL_DestroyWindow(pWindow);
				pWindow = nullptr;
			}
		}
	};

	struct FMODDeleter
	{
		void operator()(FMOD::Sound* pSound) const
		{
			if (pSound)
			{
				pSound->release();
				pSound = nullptr;
			}
		}

		void operator()(FMOD::System* pSystem) const
		{
			if (pSystem)
			{
				pSystem->release();
				pSystem = nullptr;
			}
		}
	};

	class Transform;
	struct RenderTransform
	{
		RenderTransform() = default;
		RenderTransform(const Transform& objTransform);
		RenderTransform(std::weak_ptr<Transform>&& objTransform);

		glm::fvec2 position;
		glm::fvec2 scale;
		float rotation{};
	};

	struct TextInfo
	{
		std::string text;
		uint32_t ptSize;
		glm::fvec4 fgColor;
		glm::fvec4 bgColor;
	};

	template<typename ret, typename ...Args>
	struct MulticastContainer
	{
		MulticastContainer(std::function<ret(Args...)>&& _func, Args... args)
			: func{ std::move(_func) }
			, arguments{ args... }
		{
		}

		MulticastContainer(const std::function<ret(Args...)>& _func, Args... args)
			: func{ _func }
			, arguments{ args... }
		{
		}

		std::function<ret(Args...)> func;
		std::tuple<Args...> arguments;
	};


	// For the purpose of defining types of function objects
	struct IMuticastAction
	{
		virtual ~IMuticastAction() = default;
		virtual void Invoke() = 0;
	};

	struct IMulticastCondition
	{
		virtual ~IMulticastCondition() = default;
		virtual bool Invoke() = 0;
	};

	template<typename ...Args>
	class Multicast : public IMuticastAction
	{
	public:

		Multicast(std::function<void(Args...)>&& fc, Args&&... args)
			: m_Container(std::move(fc), std::move(args...))
		{
		}

		//Multicast(const std::function<void(Args...)>& func, Args&&... args)
		//	: m_Container(func, std::move(args))
		//{
		//}

		void Invoke() override;

	private:

		MulticastContainer<void, Args...> m_Container;
	};

	template <typename ... Args>
	void Multicast<Args...>::Invoke()
	{
		std::apply(m_Container.func, m_Container.arguments);
	}

	template<typename ...Args>
	class MulticastCondition : public IMulticastCondition
	{
	public:

		//MulticastCondition(std::function<bool(Args...)>&& fc, Args&&... args)
		//	: m_Container{ std::move(fc),std::move(args...) }
		//{
		//}

		MulticastCondition(std::function<bool(Args...)>&& fc, Args... args)
			: m_Container(std::move(fc), args...)
		{
		}

		bool Invoke() override;

	private:
		MulticastContainer<bool, Args...> m_Container;
	};

	template <typename ... Args>
	bool MulticastCondition<Args...>::Invoke()
	{
		return std::apply(m_Container.func, m_Container.arguments);
	}

	template<typename T, typename ...Args>
	using Event = void(T::*)(Args...);

	class GameObject;
	template<typename UserObject = GameObject>
	struct DelegateNoParam : IMuticastAction
	{
		DelegateNoParam(void(UserObject::* functor)(), std::weak_ptr<UserObject>&& caller)
			: pFunctor(std::move(functor))
			, pCaller(std::move(caller))
		{}

		void Invoke() override;

		Event<UserObject, void> pFunctor;
		std::weak_ptr<UserObject> pCaller;
	};

	template <typename UserObject>
	void DelegateNoParam<UserObject>::Invoke()
	{
		auto validCaller{ pCaller.lock() };
		if (validCaller)
		{
			((*validCaller).*pFunctor)();
		}
	}

	template<typename UserObject = GameObject, typename ...Args>
	struct DelegateWithParams : IMuticastAction
	{
		DelegateWithParams(void(UserObject::* functor)(Args...), std::weak_ptr<UserObject>&& caller, Args... arguments)
			:pFunctor(std::move(functor))
			, pCaller(std::move(caller))
			, tArguments(arguments)
		{}

		void Invoke() override;

		Event<UserObject, Args...> pFunctor;
		std::weak_ptr<UserObject> pCaller;
		std::tuple<Args...> tArguments;
	};

	template <typename UserObject, typename ... Args>
	void DelegateWithParams<UserObject, Args...>::Invoke()
	{
		auto validCaller{ pCaller.lock() };
		if(validCaller)
		{
			((*validCaller).*pFunctor)(tArguments...);
		}
	}
}
