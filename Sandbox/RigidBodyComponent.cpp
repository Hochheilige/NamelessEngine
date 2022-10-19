#include "RigidBodyComponent.h"

btTransform RigidBodyComponent::Update()
{
    if (Body && Body->getMotionState())
    {
        Body->getMotionState()->getWorldTransform(Transform);
    }

    return Transform;
}
