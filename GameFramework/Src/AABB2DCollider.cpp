#include "AABB2DCollider.h"

bool AABB2DCollider::DoesOverlap(const AABB2DCollider& other) const
{
	Transform myTransform = GetWorldTransform();
	Transform otherTransform = other.GetWorldTransform();

	DoesOverlapRange1D(myTransform.Position.x, myTransform.Scale.x, otherTransform.Position.x, otherTransform.Scale.x );

    return DoesOverlapRange1D(myTransform.Position.x, myTransform.Scale.x, otherTransform.Position.x, otherTransform.Scale.x)
		&& DoesOverlapRange1D(myTransform.Position.y, myTransform.Scale.y, otherTransform.Position.y, otherTransform.Scale.y);
}

bool AABB2DCollider::DoesOverlap1D(float a, float b, float c, float d) const
{
	return a <= d && b >= c;
}

bool AABB2DCollider::DoesOverlapRange1D(float X, float XHalfSize, float Y, float YHalfSize) const
{
	return (X - XHalfSize <= Y + YHalfSize) && (X + XHalfSize >= Y - YHalfSize);
}
