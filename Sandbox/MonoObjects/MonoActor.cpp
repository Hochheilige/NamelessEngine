#include "MonoActor.h"
#include "MonoComponent.h"

MonoActor::MonoActor() : MonoActor("Actor") {}

MonoActor::MonoActor(const char* className) : ClassName(className)
{
    auto mono = MonoSystem::GetInstance();
    auto image = mono->GetImage();
    auto klass = mono->FindClass("Scripts", ClassName);
    CsInstance = mono->CreateClassInstance(klass);
    Handle =  mono_gchandle_new(CsInstance, true);
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
    /*auto method = mono->GetMethod("Scripts", BaseClassName, "AddComponent");
    MonoMethod* vmethod = mono_object_get_virtual_method(Handle, method);*/
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
