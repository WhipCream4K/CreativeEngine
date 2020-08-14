#include "pch.h"
#include "CreativeStructs.h"

dae::RenderTransform::RenderTransform(const Transform& objTransform)
	: position{ objTransform.GetPosition() }
	, scale{ objTransform.GetScale() }
	, rotation{ objTransform.GetRotation().z }
{
}

dae::RenderTransform::RenderTransform(std::weak_ptr<Transform>&& objTransform)
	: position(objTransform.lock()->GetPosition())
	, scale(objTransform.lock()->GetScale())
	, rotation(objTransform.lock()->GetRotation().z)
{
}
