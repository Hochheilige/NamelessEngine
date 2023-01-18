using System;
using Scripts.Components;

namespace Scripts.Engine
{
    public enum ComponentType
    {
        Undefined = -1,

        [ComponentType(typeof(MeshRendererComponent))]
        MeshRendererType = 1,

        [ComponentType(typeof(StaticMeshRenderer))]
        StaticMeshRendererType,

        [ComponentType(typeof(RigidBodyComponent))]
        RigidBodyType,

        LightPointType,

        SceneComponentType,

        [ComponentType(typeof(MovementComponent))]
        MovementComponentType,

        [ComponentType(typeof(AudioComponent))]
        AudioComponentType,

        [ComponentType(typeof(CameraComponent))]
        CameraComponentType
    }
    
    internal class ComponentTypeAttribute : Attribute
    {
        public Type Type { get; }

        public ComponentTypeAttribute(Type type)
        {
            Type = type;
        }
    }
}