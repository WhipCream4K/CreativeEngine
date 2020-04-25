#pragma once
#include <memory>

namespace dae
{
	class Transform;
	class GameObject;
	class BaseComponent
	{
		// TODO: Try to get rid of this dependencies
		friend class GameObject;
		
	public:

		BaseComponent();

		virtual ~BaseComponent() = default;
		BaseComponent(const BaseComponent&) = delete;
		BaseComponent(BaseComponent&&) noexcept = delete;
		BaseComponent& operator=(const BaseComponent&) = delete;
		BaseComponent& operator=(BaseComponent&&) noexcept = delete;

		void SetActive(bool value) { m_IsActive = value; }
		bool IsActive() { return m_IsActive; }
		
		std::shared_ptr<GameObject> GetGameObject() const { return m_pGameObject.lock(); }

	protected:
		
		virtual void Render() {}
		
		virtual void Awake() {}
		virtual void Start() = 0;
		
		virtual void Update() = 0;
		virtual void LateUpdate() {}

	private:
		std::weak_ptr<GameObject> m_pGameObject;
		bool m_IsActive;
	};
}

