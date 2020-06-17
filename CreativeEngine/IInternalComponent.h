#pragma once

namespace dae
{
	class IInternalComponent
	{
	public:

		virtual ~IInternalComponent() = default;
		
		virtual void Render() const = 0;
		virtual void Awake() = 0;
		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual void LateUpdate() = 0;

		virtual void RegisterOwner(std::weak_ptr<GameObject>&&) = 0;
	};
}

