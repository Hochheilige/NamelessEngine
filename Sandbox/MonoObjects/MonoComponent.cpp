#include "MonoComponent.h"

void MonoComponent::ConstructFromCsInstance(MonoObject* instance, Component* component)
{
    Handle =  mono_gchandle_new(instance, true);
    
    auto mono = MonoSystem::GetInstance();
    void *args [1];
    args [0] = &component;
    
    MonoMethod* method = mono->GetVirtualMethod("Scripts", "Component", "SetCppInstance", Handle);
    MonoObject* result = mono->InvokeInstanceMethod(method, Handle, args, nullptr);
}

MonoComponent::~MonoComponent()
{
    if(mono_gchandle_get_target(Handle) != nullptr)
    {
        auto mono = MonoSystem::GetInstance();
    
        MonoMethod* method = mono->GetVirtualMethod("Scripts", "Component", "Dispose", Handle);
        MonoObject* result = mono->InvokeInstanceMethod(method, Handle, nullptr, nullptr);
    }
}
