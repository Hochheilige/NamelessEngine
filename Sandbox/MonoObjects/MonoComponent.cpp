#include "MonoComponent.h"

void MonoComponent::ConstructFromCsInstance(MonoObject* instance, Component* component)
{
    Handle =  mono_gchandle_new(instance, true);
    
    auto mono = MonoSystem::GetInstance();
    void *args [1];
    args [0] = &component;
    
    MonoMethod* method = mono->GetVirtualMethod("Scripts", "Component", "SetCppInstance", mono_gchandle_get_target (Handle));
    MonoObject* result = mono->InvokeMethod(method, mono_gchandle_get_target (Handle), args, nullptr);
}

MonoComponent::~MonoComponent()
{
    auto mono = MonoSystem::GetInstance();
    
    MonoMethod* method = mono->GetVirtualMethod("Scripts", "Component", "Dispose", mono_gchandle_get_target (Handle));
    MonoObject* result = mono->InvokeMethod(method, mono_gchandle_get_target (Handle), nullptr, nullptr);
}
