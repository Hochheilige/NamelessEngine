#pragma once

#include "..\MonoSystem.h"
#include "..\Component.h"

class MonoActor
{
public:
    MonoActor();
    void AddComponent(Component* component);
    void RemoveComponent(Component* component);
    const char* GetInheritors();
private:
    MonoObject* CsInstance = nullptr;
};
