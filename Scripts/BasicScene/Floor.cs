using System;
using Scripts.Components;

namespace Scripts.BasicScene
{
    public class Floor : Actor
    {
        private RigidBodyComponent cubeComponent;
        private StaticMeshRenderer renderer;

        protected internal override void Init()
        {
            cubeComponent = AddComponent<RigidBodyComponent>("RigidBodyCube");
            cubeComponent.SetUsage(RigidBodyUsage.CollisionsAndPhysics);
            cubeComponent.SetCollisionShape(CollisionShape.Box);
            cubeComponent.SetType(RigidBodyType.Static);
            cubeComponent.SetMass(0);
            cubeComponent.EnablePhysicsSimulation();

            renderer = AddComponent<StaticMeshRenderer>("StaticMeshRender");
            renderer.SetMeshPath("../Assets/box.fbx/Cube");
            renderer.SetTransform(GetTransform());
        }

        public override void OnBeginPlay()
        {
        }
    }
}