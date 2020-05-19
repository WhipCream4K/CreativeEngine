#pragma once
#include <string>
#include <vector>

#include "Transform.h"

namespace dae
{
	template<typename T>
	//using GameComponent = std::shared_ptr<typename std::enable_if<std::is_base_of_v<BaseComponent, T>, T>::type>;
	using GameComponent = typename std::enable_if<std::is_base_of_v<BaseComponent, T>, T>::type;
	
	class Scene;
	class BaseComponent;
	class GameObject
	{
	friend class Scene;  
	public:
		
		GameObject();
		virtual ~GameObject() = default;
		GameObject(const GameObject&) = delete;
		GameObject(GameObject&&) = delete;
		GameObject& operator=(const GameObject&) = delete;
		GameObject& operator=(GameObject&&) = delete;

		void SetActive(bool value) { m_IsActive = value; }

		template<typename T>
		constexpr std::shared_ptr<GameComponent<T>> GetComponent();
		
		template<typename T,typename ... Args>
		constexpr std::shared_ptr<GameComponent<T>> CreateComponent(Args... args);

		std::shared_ptr<Scene> GetScene() const { return m_pRefScene.lock(); }

	protected:

		virtual void Render() const;
		
		virtual void Awake() {}
		virtual void Start() = 0;

		virtual void Update() = 0;
		virtual void LateUpdate() {}

	private:

		Transform m_Transform;
		std::vector<std::shared_ptr<BaseComponent>> m_pComponents;
		std::weak_ptr<Scene> m_pRefScene;
		std::string m_Tag;
		bool m_IsStatic;
		bool m_IsActive;

	};


	template <typename T>
	constexpr std::shared_ptr<GameComponent<T>> GameObject::GetComponent()
	{
		auto& type = typeid(T);
		for (auto& component : m_pComponents)
		{
			if (typeid(component) == type)
				return std::static_pointer_cast<T, BaseComponent>(component);
		}

		return nullptr;
	}

	template <typename T,typename ... Args>
	constexpr std::shared_ptr<GameComponent<T>> GameObject::CreateComponent(Args... args)
	{
		std::shared_ptr<T> component{ std::make_shared<T>(new T(args)) };
		m_pComponents.push_back(component);
		return component; // return strong ref to this component
	}
}

