using System;
using System.Linq;
using Scripts.Components;
using Scripts.Tests;
using SharpDX;

namespace Scripts
{
    public class Bullet : Actor
    {
        private StaticMeshRenderer renderer;
        private MovementComponent mv_cmp;
        private RigidBodyComponent rigidbody;
        private AudioComponent audio;

        private float lifespan = 2.0f;
        private Vector3 initialSpeed = new Vector3(10.0f, 10.0f, 10.0f);
      
        public Bullet()
        {
            
        }

        public void PublicInit()
        {
            Init();
        }

        protected internal override void Init()
        {
            //rigidbody = (RigidBodyComponent)AddComponent(ComponentType.RigidBodyCubeType);
            rigidbody = AddComponent<RigidBodySphereComponent>("RigidBodyComponent");
            rigidbody.SetType(RigidBodyType.Dynamic);
            rigidbody.SetUsage(RigidBodyUsage.CollisionsAndPhysics);
            rigidbody.SetCollisionShape(CollisionShape.Sphere);
            rigidbody.SetMass(1);
            rigidbody.EnablePhysicsSimulation();
            //mv_cmp = (MovementComponent)AddComponent((int)ComponentsEnum.MovementComponentType);
            // add a simple rigid body instead and activate physics after a slight offset to avoid collision with playah
            // gotta add the ability to activate/deactivate physics from here
            // and set impulse

            //renderer = (StaticMeshRenderer)AddComponent(ComponentType.StaticMeshRendererType);
            renderer = AddComponent<StaticMeshRenderer>("StaticMeshRenderer");
            renderer.SetMeshPath("../Assets/box.fbx/Cube");

            audio = AddComponent<AudioComponent>("Audio");
            audio.LoadSound("../Assets/explosion.mp3", true, false);
            audio.SetVolume(0.5f);
        }

        public override void Update(float deltaTime)
        {
            lifespan -= deltaTime;

            if (lifespan < 0.0f)
            {
                Destroy();
            }
            //Move();
        }

        public override void OnBeginPlay()
        {
            Console.WriteLine("Bullet Spawned");
            //rigidbody.ApplyCentralImpulse(initialSpeed);

            //mv_cmp.Jump(initialSpeed);
        }

        internal override void OnGUI()
        {
            
        }

        public void fire()
        {
            rigidbody.ApplyCentralImpulse(initialSpeed);
            audio.Play();
        }

        public void SetInittialSpeed(Vector3 speed)
        {
            initialSpeed = speed;
        }

        private void Move()
        {
            var root = GetTransform();
            root.Position.X += 0.001f;
            root.Position.X += 0.001f;
            root.Position.X += 0.001f;
            SetTransform(root);
        }
    }
}