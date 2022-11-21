#include "MonoActor.h"

#include <iostream>

#include "MonoComponent.h"

MonoActor::MonoActor(const char* className, Actor* actor) : ClassName(className)
{
    auto mono = MonoSystem::GetInstance();
    auto image = mono->GetImage();
    owner = actor;
    auto klass = mono->FindClass("Scripts", ClassName);
    CsInstance = mono->CreateClassInstance(klass);
    Handle =  mono_gchandle_new(CsInstance, true);
    
    void *args [1];
    args [0] = &owner;
    std::cout << "SMOTRI C++" << owner;
    
    MonoMethod* method = mono->GetVirtualMethod("Scripts", BaseClassName, "SetCppInstance", CsInstance);
    MonoObject* result = mono->InvokeMethod(method, mono_gchandle_get_target (Handle), args, nullptr);
    
}
/*void MonoComponent::SetCsInstance(MonoObject* instance)
{
    CsInstance = instance;
    
    auto mono = MonoSystem::GetInstance();
    void *args [1];
    args [0] = this;
    
    MonoMethod* method = mono->GetVirtualMethod("Scripts", "Component", "SetCppInstance", CsInstance);
    MonoObject* result = mono->InvokeMethod(method, CsInstance, args, nullptr);
}*/
MonoActor::MonoActor() : MonoActor("Actor", nullptr) {}

void MonoActor::AddComponent(Component* component)
{
    auto type = component->GetComponentType();
    auto monoComp = component->GetMonoComponent();
   
    auto mono = MonoSystem::GetInstance();
    
    void *args [1];
    args [0] = &type;

    MonoMethod* method = mono->GetVirtualMethod("Scripts", BaseClassName, "AddComponent", mono_gchandle_get_target (Handle));
    /*auto method = mono->GetMethod("Scripts", BaseClassName, "AddComponent");
    MonoMethod* vmethod = mono_object_get_virtual_method(Handle, method);*/
    MonoObject* result = mono->InvokeMethod(method, mono_gchandle_get_target (Handle), args, nullptr);

    monoComp->SetCsInstance(result);
}

void MonoActor::RemoveComponent(Component* component)
{
}

void MonoActor::Update(float deltaTime)
{
    if(!isWorked)
    {
        OnBeginPlay();
        isWorked = true;
    }
    
    auto mono = MonoSystem::GetInstance();
    void *args [1];
    args [0] = &deltaTime;
    
    //MonoMethod* method = mono->GetVirtualMethod("Scripts", ClassName, "Update", CsInstance);
    MonoMethod* method = mono->GetMethod("Scripts", ClassName, "Update");
    MonoObject* result = mono->InvokeMethod(method, mono_gchandle_get_target (Handle), args, nullptr);

}

void MonoActor::OnBeginPlay()
{
    auto mono = MonoSystem::GetInstance();

    MonoMethod* method = mono->GetMethod("Scripts", ClassName, "OnBeginPlay");
    MonoObject* result = mono->InvokeMethod(method, mono_gchandle_get_target (Handle), nullptr, nullptr);
}

const char* MonoActor::GetInheritors()
{
    auto mono = MonoSystem::GetInstance();
    
    MonoMethod* method = mono->GetMethod("Scripts", ClassName, "GetInheritors");
    MonoObject* result = mono->InvokeMethod(method, mono_gchandle_get_target (Handle), nullptr, nullptr);
    auto str = mono_string_to_utf8(mono_object_to_string(result, nullptr));
    return str;
}
