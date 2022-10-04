#pragma once

#include "Collider.h"

class AABB2DCollider : public Collider
{
public:

	COLLIDER_CHILD_BODY()

	virtual bool DoesOverlap(const AABB2DCollider& Other) const override;

private:
	bool DoesOverlap1D(float a, float b, float c, float d) const;
	bool DoesOverlapRange1D(float X, float XHalfSize, float Y, float YHalfSize) const;
};

