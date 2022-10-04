#pragma once

#include "MathInclude.h"
#include "GameComponent.h"

#define COLLIDER_CHILD_BODY() \
public: \
virtual bool DoesOverlap(const Collider& Other) const override \
{ \
	return Other.DoesOverlap(*this); \
} \

class Collider : public GameComponent
{
public:

	virtual bool DoesOverlap(const Collider& Other) const = 0;

	virtual bool DoesOverlap(const class AABB2DCollider& Other) const;

	virtual bool DoesOverlap(const class SphereCollider& Other) const;
};