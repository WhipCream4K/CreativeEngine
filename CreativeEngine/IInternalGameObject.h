#pragma once
#include "IInternalSceneObject.h"

namespace dae
{
	class IInternalGameObject : public IInternalSceneObject
	{
	public:

		virtual ~IInternalGameObject() = default;

		virtual void SetOwner(std::weak_ptr<Scene>) = 0;
		virtual void RootAwake() = 0;
		virtual void RootStart() = 0;
		virtual void RootLateUpdate() = 0;
	};
}

