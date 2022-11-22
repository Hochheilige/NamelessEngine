#include "MonoActor.h"

#include "MonoComponent.h"

MonoActor::MonoActor(Actor* actor, const char* className) : ClassName(className)
{
    auto mono = MonoSystem::GetInstance();
    auto klass = mono->FindClass("Scripts", ClassName);
    
    Owner = actor;
    void *args [1];
    args [0] = &Owner;

    auto csInstance = mono->CreateClassInstance(klass, false);
    Handle =  mono_gchandle_new(csInstance, true);
    
    MonoMethod* method = mono->GetVirtualMethod("Scripts", BaseClassName, "SetCppInstance", mono_gchandle_get_target (Handle));
    mono->InvokeMethod(method, mono_gchandle_get_target (Handle), args, nullptr);
    
    mono_runtime_object_init(mono_gchandle_get_target (Handle));
}

MonoActor::~MonoActor()
{
    auto mono = MonoSystem::GetInstance();
    MonoMethod* method = mono->GetVirtualMethod("Scripts", BaseClassName, "Dispose", mono_gchandle_get_target (Handle));
    mono->InvokeMethod(method, mono_gchandle_get_target (Handle), nullptr, nullptr);
}

void MonoActor::AddComponent(Component* component)
{
    auto type = component->GetComponentType();
    auto monoComp = component->GetMonoComponent();
   
    auto mono = MonoSystem::GetInstance();
    
    void *args [1];
    args [0] = &type;

    MonoMethod* method = mono->GetVirtualMethod("Scripts", BaseClassName, "AddComponent", mono_gchandle_get_target (Handle));
    MonoObject* result = mono->InvokeMethod(method, mono_gchandle_get_target (Handle), args, nullptr);

    monoComp->ConstructFromCsInstance(result);
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

MonoObject* MonoActor::GetCsInstance()
{
    return mono_gchandle_get_target (Handle);
}
