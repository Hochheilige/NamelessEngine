#include "MonoActor.h"
#include "MonoComponent.h"

MonoActor::MonoActor()
{
    auto mono = MonoSystem::GetInstance();
    auto image = mono->GetImage();
    auto klass = mono->FindClass("Scripts", "Actor");
    CsInstance = mono->CreateClassInstance(klass);
}

void MonoActor::AddComponent(Component* component)
{
    auto type = component->GetComponentType();
    auto monoComp = component->GetMonoComponent();
   
    auto mono = MonoSystem::GetInstance();
    
    void *args [1];
    args [0] = &type;
    
    MonoMethod* method = mono->GetMethod("Scripts", "Actor", "AddComponent");
    MonoObject* result = mono->InvokeMethod(method, CsInstance, args, nullptr);

    monoComp->SetCsInstance(result);
}

void MonoActor::RemoveComponent(Component* component)
{
}
