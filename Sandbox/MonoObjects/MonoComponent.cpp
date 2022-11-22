#include "MonoComponent.h"

void MonoComponent::ConstructFromCsInstance(MonoObject* instance)
{
    CsInstance = instance;
    Handle =  mono_gchandle_new(CsInstance, true);
    
    auto mono = MonoSystem::GetInstance();
    void *args [1];
    args [0] = this;
    
    MonoMethod* method = mono->GetVirtualMethod("Scripts", "Component", "SetCppInstance", CsInstance);
    MonoObject* result = mono->InvokeMethod(method, mono_gchandle_get_target (Handle), args, nullptr);
}

MonoComponent::~MonoComponent()
{
    auto mono = MonoSystem::GetInstance();
    
    MonoMethod* method = mono->GetVirtualMethod("Scripts", "Component", "Dispose", CsInstance);
    MonoObject* result = mono->InvokeMethod(method, mono_gchandle_get_target (Handle), nullptr, nullptr);
}
