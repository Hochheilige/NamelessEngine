#pragma once
#include "MonoSystem.h"
#include <Transform.h>

class ScriptObject
{
public:
    ScriptObject(const char* name, MonoSystem* mono);

    void Update(float deltaTime);

    Transform GetTransform();

private:
    const char* _name;
    
    MonoSystem* _mono;

    MonoClass* _klass;

    MonoObject* _instance;
};

