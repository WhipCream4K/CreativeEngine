#pragma once
class IInternalSceneObject
{
public:
	virtual ~IInternalSceneObject() = default;

	virtual void RootInitialize() = 0;
	virtual void RootRender() const = 0;
	virtual void RootUpdate() = 0;
};

