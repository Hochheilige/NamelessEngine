#pragma once
#include "..\MonoSystem.h"
#include "..\Component.h"

class MonoComponent
{
public:
    void ConstructFromCsInstance(MonoObject* instance, Component* component);

    ~MonoComponent();
protected:
    uint32_t Handle;
    //MonoObject* CsInstance = nullptr;
};
