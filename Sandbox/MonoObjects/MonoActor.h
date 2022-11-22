#pragma once

#include "..\MonoSystem.h"
#include "..\Component.h"

class MonoActor
{
public:
    MonoActor();
    MonoActor(const char* className);

    ~MonoActor();
    
    void AddComponent(Component* component);
    void RemoveComponent(Component* component);
    void Update(float deltaTime);
    void OnBeginPlay();
    
    const char* GetInheritors();
private:
    uint32_t Handle;
    MonoObject* CsInstance = nullptr;
    const char* ClassName = "CustomActor";
    const char* BaseClassName = "Actor";
};
