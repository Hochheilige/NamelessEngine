#pragma once

#include "Transform.h"
#include "../Actor.h"
#include "../MonoSystem.h"
#include "../RigidBodyComponent.h"
#include "MovementComponent.h"
#include "ComponentRegistry.h"
#include "InputDevice.h"

class MonoMasterModule
{
public:
    MonoMasterModule()
    {
        mono_add_internal_call("Scripts.Internal.ExternalApi::IsMouseDown", &IsMouseDown);
    }

private:
    static bool IsMouseDown(int button)
    {
        return Game::GetInstance()->GetInputDevice()->GetMouse()->IsDown(static_cast<Button>(button));

    }
};
