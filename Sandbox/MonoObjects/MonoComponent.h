#pragma once
#include "MonoActor.h"

class MonoComponent
{
public:
    void SetCsInstance(MonoObject* instance);
protected:
    MonoObject* CsInstance = nullptr;
};
