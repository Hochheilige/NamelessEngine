#pragma once

#include "StaticMeshRenderer.h"

class MonoStaticMeshRendererModule
{
public:
    MonoStaticMeshRendererModule()
    {
        mono_add_internal_call("Scripts.Components.StaticMeshRenderer::InternalSetMeshPath", &SetMeshPath);
        mono_add_internal_call("Scripts.Components.StaticMeshRenderer::InternalSetTexturePath", &SetTexturePath);
    }

private:
    static void SetMeshPath(StaticMeshRenderer* component, MonoObject* meshPath)
    {   
        auto str = mono_string_to_utf8(mono_object_to_string(meshPath, nullptr));
        component->SetMeshPath(str);
    }

    static void SetTexturePath(StaticMeshRenderer* component, MonoObject* texturePath)
    {   
        auto str = mono_string_to_utf8(mono_object_to_string(texturePath, nullptr));
        component->SetTexturePath(str);
    }
};
