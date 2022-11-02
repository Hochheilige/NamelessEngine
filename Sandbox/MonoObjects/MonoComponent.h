#pragma once
#include "..\MonoSystem.h"

class MonoComponent
{
public:
    void SetCsInstance(MonoObject* instance);
protected:
    MonoObject* CsInstance = nullptr;
};
