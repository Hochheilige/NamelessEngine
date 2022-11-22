#pragma once
#include "..\MonoSystem.h"

class MonoComponent
{
public:
    void ConstructFromCsInstance(MonoObject* instance);

    ~MonoComponent();
protected:
    uint32_t Handle;
    //MonoObject* CsInstance = nullptr;
};
