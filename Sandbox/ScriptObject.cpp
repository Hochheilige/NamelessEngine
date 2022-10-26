#include "ScriptObject.h"

ScriptObject::ScriptObject(const char* name, MonoSystem* mono)
{
    _mono = mono;
    _name = name;
    auto image = _mono->GetImage();
    _klass = _mono->FindClass("Scripts", name);
    _instance = _mono->CreateClassInstance(_klass);
}

void ScriptObject::Update(float deltaTime)
{
    void *args [1];
    args [0] = &deltaTime;
    MonoMethod* method = _mono->GetMethod("Scripts", _name, "Update");
    _mono->InvokeMethod(method, _instance, args, nullptr);
}

Transform ScriptObject::GetTransform()
{
    MonoMethod* method = _mono->GetVirtualMethod("Scripts", _name, "GetTransform", _instance);
    MonoObject* result = _mono->InvokeMethod(method, _instance, nullptr, nullptr);

    return *(Transform*)mono_object_unbox(result);
}
