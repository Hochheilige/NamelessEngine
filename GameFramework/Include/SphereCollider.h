#pragma once

#include "Collider.h"

class SphereCollider : public Collider
{
	COLLIDER_CHILD_BODY()

public:

	float Radius = 1.0f;

	virtual bool DoesOverlap(const SphereCollider& Other) const override;
};