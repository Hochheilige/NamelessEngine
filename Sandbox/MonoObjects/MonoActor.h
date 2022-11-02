#pragma once

#include "MonoComponent.h"
#include "Transform.h"
#include "..\MonoSystem.h"

class MonoActor
{
public:
    MonoActor();
    void SetTransform(Transform transform);
    void AddComponent(MonoComponent* component);
private:
    MonoObject* CsInstance = nullptr;
};
