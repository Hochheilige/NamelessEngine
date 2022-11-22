#include "MonoPhysicsComponent.h"

void MonoPhysicsComponent::SetMass(float mass)
{
    auto mono = MonoSystem::GetInstance();
    void *args [1];
    args [0] = &mass;

    MonoMethod* method = mono->GetMethod("Scripts", "PhysicsComponent", "SetMass");
    mono->InvokeMethod(method, mono_gchandle_get_target (Handle), args, nullptr);
}
