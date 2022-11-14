#pragma once

#include "..\MonoSystem.h"
#include "..\Component.h"

class MonoActor
{
public:
    MonoActor();
    MonoActor(const char* className);
    void AddComponent(Component* component);
    void RemoveComponent(Component* component);
    void Update(float deltaTime);
    void OnBeginPlay();
    
    const char* GetInheritors();
private:
    MonoObject* CsInstance = nullptr;
    const char* ClassName = "CustomActor";
    const char* BaseClassName = "Actor";
};
