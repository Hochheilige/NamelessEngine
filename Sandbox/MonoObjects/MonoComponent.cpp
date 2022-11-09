#include "MonoComponent.h"

void MonoComponent::SetCsInstance(MonoObject* instance)
{
    CsInstance = instance;
    
    auto mono = MonoSystem::GetInstance();
    void *args [1];
    args [0] = this;
    
    MonoMethod* method = mono->GetVirtualMethod("Scripts", "Component", "SetCppInstance", CsInstance);
    MonoObject* result = mono->InvokeMethod(method, CsInstance, args, nullptr);
}
