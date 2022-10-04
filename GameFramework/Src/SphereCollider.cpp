#include "SphereCollider.h"

bool SphereCollider::DoesOverlap(const SphereCollider& Other) const
{
	const float distanceSqr = Vector3::DistanceSquared(GetWorldTransform().Position, Other.GetWorldTransform().Position);
	const float radSum = Radius + Other.Radius;
	return distanceSqr <= radSum * radSum;
}