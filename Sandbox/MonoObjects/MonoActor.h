#pragma once

#include "..\MonoSystem.h"
#include "..\Component.h"

class Actor;

class MonoActor
{
public:
    ~MonoActor();
    
    MonoActor(Actor* actor, const char* nameSpace, const char* className);
    void RegisterComponents();
    void Init();

    void AddComponent(Component* component);
    void RemoveComponent(Component* component);
    void Update(float deltaTime);
	void OnGui();
    void OnBeginPlay();

    void Hit(std::shared_ptr<Actor> otherActor);
    void BeginOverlap(std::shared_ptr<Actor> otherActor);
    void EndOverlap(std::shared_ptr<Actor> otherActor);
    
    const char* GetInheritors();
    MonoObject* GetCsInstance();

    const std::string& GetNamespace() const;
    const std::string& GetClassname() const;
private:
    Actor* Owner;
    uint32_t Handle;
    //MonoObject* CsInstance = nullptr;
    std::string NameSpace;
    std::string ClassName;
    const char* BaseClassName = "Actor";
    bool isWorked = false;
};
