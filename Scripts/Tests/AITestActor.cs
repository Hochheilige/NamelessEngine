using Scripts.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Scripts.BehaviorTree;

namespace Scripts.Tests
{
    public class AITestActor : Actor
    {

        StaticMeshRenderer meshRenderer;
        BehaviorTreeComponent btComp;
        RigidBodyComponent rigidbody;
        protected internal override void Init()
        {
            AddComponent<MovementComponent>();
            //rigidbody = AddComponent<RigidBodySphereComponent>("RigidBody");
            //rigidbody.SetType(RigidBodyType.Dynamic);
            //rigidbody.SetUsage(RigidBodyUsage.CollisionsAndPhysics);
            //rigidbody.SetCollisionShape(CollisionShape.Sphere);
            //rigidbody.SetMass(1);
            //rigidbody.EnablePhysicsSimulation();

            meshRenderer = AddComponent<StaticMeshRenderer>("Mesh Renderer");
            meshRenderer.SetMeshPath("../Assets/box.fbx/Cube");
            btComp = new BehaviorTreeComponent(this);

            //BTSequence sequence = new BTSequence();
            //tree.SetRoot(sequence);
            //BTTask_Wait wait = new BTTask_Wait();
            //sequence.AddChild(wait);
            //BTTask_RandomMove move = new BTTask_RandomMove();
            //sequence.AddChild(move);
            btComp.SetBTTree(BehaviorTreeManager.Instance.GetTree("../Assets/AI/BehaviorTree0.json"));
        }

        public override void Update(float deltaTime)
        {
            btComp.GetExecutor().Execute(deltaTime);
        }
    }
}
