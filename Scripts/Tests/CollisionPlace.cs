using System;
using System.Linq;
using Scripts.Components;
using Scripts.Tests;
using SharpDX;

namespace Scripts
{
    public class CollisionPlace : Actor
    {
        private RigidBodyComponent rigidbody;
        public CollisionPlace()
        {
            
        }

        public void PublicInit()
        {
            Init();
        }

        protected internal override void Init()
        {
            //rigidbody = (RigidBodyComponent)AddComponent(ComponentType.RigidBodyCubeType);
            rigidbody = AddComponent<RigidBodyComponent>("RigidBodyComponent");
            rigidbody.SetType(RigidBodyType.Static);
            rigidbody.SetUsage(RigidBodyUsage.OverlapAll);
            rigidbody.SetCollisionShape(CollisionShape.Sphere);
            rigidbody.SetMass(0);
            rigidbody.GenerateOverlapEvents(true);
            //rigidbody.GenerateHitEvents(true);
            rigidbody.EnablePhysicsSimulation();
            //mv_cmp = (MovementComponent)AddComponent((int)ComponentsEnum.MovementComponentType);
            // add a simple rigid body instead and activate physics after a slight offset to avoid collision with playah
            // gotta add the ability to activate/deactivate physics from here
            // and set impulse


        }

        public override void Update(float deltaTime)
        {

        }

        public override void OnBeginPlay()
        {
            //Console.WriteLine("Bullet Spawned");
            //rigidbody.ApplyCentralImpulse(initialSpeed);

            //mv_cmp.Jump(initialSpeed);
        }

        internal override void OnGUI()
        {
            
        }


        protected override void BeginOverlap(Actor otherActor)
        {
            //otherActor.Components.GetComponent<StaticMeshRenderer>().SetMeshPath("../Assets/Meshes/test.fbx/Torus");
            Console.WriteLine("Collision place BeginOverlap event with {0}", otherActor.ToString());
        }        
        protected override void EndOverlap(Actor otherActor)
        {
            //otherActor.Components.GetComponent<StaticMeshRenderer>().SetMeshPath("../Assets/Meshes/test.fbx/Torus");
            Console.WriteLine("Collision place EndOverlap event with {0}", otherActor.ToString());
        }

        protected override void Hit(Actor otherActor)
        {

            Console.WriteLine("Collision place Hit event with {0}", otherActor.ToString());
        }

    }
}