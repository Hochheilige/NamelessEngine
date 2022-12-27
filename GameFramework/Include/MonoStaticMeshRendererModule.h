#pragma once

#include "StaticMeshRenderer.h"

class MonoStaticMeshRendererModule
{
public:
    MonoStaticMeshRendererModule()
    {
        mono_add_internal_call("Scripts.Components.StaticMeshRenderer::InternalSetMeshPath", &SetMeshPath);
    }

private:
    static void SetMeshPath(StaticMeshRenderer* component, MonoObject* meshPath)
    {   
        auto str = mono_string_to_utf8(mono_object_to_string(meshPath, nullptr));
        component->SetMeshPath(str);
    }
};
