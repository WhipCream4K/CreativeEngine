#include "pch.h"
#include "Transform.h"
#include "CreativeStructs.h"

dae::RenderTransform::RenderTransform(const Transform& objTransform)
	: position{ objTransform.GetPosition() }
	, scale{ objTransform.GetScale() }
	, rotation{ objTransform.GetRotation().z }
{
}
