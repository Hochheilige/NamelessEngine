#include "MonoPhysicsComponent.h"

void MonoPhysicsComponent::SetMass(float mass)
{
    auto mono = MonoSystem::GetInstance();
    void *args [1];
    args [0] = &mass;

    MonoMethod* method = mono->GetMethod("Scripts", "PhysicsComponent", "SetMass");
    mono->InvokeInstanceMethod(method, Handle, args, nullptr);
}
