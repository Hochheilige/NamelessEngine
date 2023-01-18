#pragma once

// Should be Undefined for abstract components, abstract components can't have instances
// Any other type should be registered in ComponentRegistry 
enum ComponentType
{
    Undefined = -1,

    MeshRendererType = 1,
    StaticMeshRendererType,
    RigidBodyComponentType, 
    LightPointType,
    SceneComponentType,
    MovementComponentType,
    AudioComponentType,
	CameraComponentType,
    AmbientLightType,
    DirectionalLightType,

    Last
};
