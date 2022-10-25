#include "ScriptObject.h"

ScriptObject::ScriptObject(const char* name, MonoSystem* mono)
{
    _mono = mono;
    auto image = _mono->GetImage();
    _klass = mono_class_from_name(image, "Scripts", "Cargo");
    _instance = _mono->CreateClassInstance(_klass);
}

void ScriptObject::Update(float deltaTime)
{
    void *args [1];
    args [0] = &deltaTime;
    MonoMethod* method = _mono->GetMethod("Scripts", "Cargo", "Scripts.Cargo:Update");
    //MonoMethod* method = _mono->GetVirtualMethod("Scripts", "Cargo", "Scripts.Cargo:Update(float)", _instance);
    _mono->InvokeMethod(method, _instance, args, nullptr);
}

Transform ScriptObject::GetTransform()
{
    MonoMethodDesc* methodDesc = _mono->MakeMethodDescriptor("Scripts.Cargo:GetTransform()", true);
    MonoMethod* method = _mono->GetVirtualMethod("Scripts", "Cargo", "Scripts.Cargo:GetTransform()", _instance);
    MonoObject* result = _mono->InvokeMethod(method, _instance, nullptr, nullptr);

    return *(Transform*)mono_object_unbox(result);
}
