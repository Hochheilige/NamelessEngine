using System;
using Scripts.Components;

namespace Scripts.BasicScene
{
    public class Floor : Actor
    {
        private RigidBodyCubeComponent cubeComponent;
        private StaticMeshRenderer renderer;

        private protected override void RegisterComponents()
        {
            cubeComponent = AddComponent<RigidBodyCubeComponent>("RigidBodyCube");
            cubeComponent.SetUsage(RigidBodyUsage.CollisionsAndPhysics);
            cubeComponent.SetCollisionShape(CollisionShape.Box);
            cubeComponent.SetType(RigidBodyType.Static);
            cubeComponent.SetMass(1);
            cubeComponent.SetGravity(0);
            cubeComponent.EnablePhysicsSimulation();
            
            renderer = AddComponent<StaticMeshRenderer>("StaticMeshRender");
            renderer.SetMeshPath("../Assets/box.fbx/Cube");
            renderer.SetTransform(GetTransform());
            
            Console.WriteLine("Register Floor");
        }

        private protected override void Init()
        {
            cubeComponent = AddComponent<RigidBodyCubeComponent>("RigidBodyCube");
            renderer = AddComponent<StaticMeshRenderer>("StaticMeshRender");
            
            Console.WriteLine("Init Floor");
        }

        public override void OnBeginPlay()
        {
            Console.WriteLine("Begin derived");
        }
    }
}