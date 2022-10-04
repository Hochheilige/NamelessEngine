#include "..\Include\Collider.h"

bool Collider::DoesOverlap(const AABB2DCollider& Other) const
{
	return false;
}

bool Collider::DoesOverlap(const SphereCollider& Other) const
{
	return false;
}
