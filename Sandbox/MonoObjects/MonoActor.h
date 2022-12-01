#pragma once

#include "..\MonoSystem.h"
#include "..\Component.h"

class MonoActor
{
public:
    ~MonoActor();
    
    MonoActor(Actor* actor, const char* nameSpace, const char* className);
    
    void AddComponent(Component* component);
    void RemoveComponent(Component* component);
    void Update(float deltaTime);
    void OnBeginPlay();
    
    const char* GetInheritors();
    MonoObject* GetCsInstance();
private:
    Actor* Owner;
    uint32_t Handle;
    //MonoObject* CsInstance = nullptr;
    const char* NameSpace;
    const char* ClassName;
    const char* BaseClassName = "Actor";
    bool isWorked = false;
};
