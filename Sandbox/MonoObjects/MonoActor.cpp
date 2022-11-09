#include "MonoActor.h"
#include "MonoComponent.h"

MonoActor::MonoActor()
{
    auto mono = MonoSystem::GetInstance();
    auto image = mono->GetImage();
    auto klass = mono->FindClass("Scripts", ClassName);
    CsInstance = mono->CreateClassInstance(klass);
}

MonoActor::MonoActor(const char* className) : ClassName(className){}

void MonoActor::AddComponent(Component* component)
{
    auto type = component->GetComponentType();
    auto monoComp = component->GetMonoComponent();
   
    auto mono = MonoSystem::GetInstance();
    
    void *args [1];
    args [0] = &type;

    MonoMethod* method = mono->GetVirtualMethod("Scripts", BaseClassName, "AddComponent", CsInstance);
    MonoObject* result = mono->InvokeMethod(method, CsInstance, args, nullptr);

    monoComp->SetCsInstance(result);
}

void MonoActor::RemoveComponent(Component* component)
{
}

void MonoActor::Update(float deltaTime)
{
    auto mono = MonoSystem::GetInstance();
    
    void *args [1];
    args [0] = &deltaTime;
    
    //MonoMethod* method = mono->GetVirtualMethod("Scripts", ClassName, "Update", CsInstance);
    MonoMethod* method = mono->GetMethod("Scripts", ClassName, "Update");
    MonoObject* result = mono->InvokeMethod(method, CsInstance, args, nullptr);
}

const char* MonoActor::GetInheritors()
{
    auto mono = MonoSystem::GetInstance();
    
    MonoMethod* method = mono->GetMethod("Scripts", ClassName, "GetInheritors");
    MonoObject* result = mono->InvokeMethod(method, CsInstance, nullptr, nullptr);
    auto str = mono_string_to_utf8(mono_object_to_string(result, nullptr));
    return str;
}
