using System;
using Scripts.Components;
using SharpDX;

namespace Scripts
{
    public class Bullet : Actor
    {

        private StaticMeshRenderer renderer;
        private MovementComponent mv_cmp;
        private RigidBodyComponent rigidbody;

        private float lifespan = 4.0f;
        private Vector3 initialSpeed = new Vector3(10.0f, 10.0f, 10.0f);
      
        public Bullet()
        {
            rigidbody = (RigidBodyComponent)AddComponent((int)ComponentsEnum.RigidBodyCubeType);
            rigidbody.EnablePhysicsSimulation();
            //mv_cmp = (MovementComponent)AddComponent((int)ComponentsEnum.MovementComponentType);
            // add a simple rigid body instead and activate physics after a slight offset to avoid collision with playah
            // gotta add the ability to activate/deactivate physics from here
            // and set impulse

            renderer = (StaticMeshRenderer)AddComponent((int)ComponentsEnum.StaticMeshRendererType);
            renderer.SetMeshPath("../Assets/box.fbx/Cube");
        }
        
        public override void Update(float deltaTime)
        {
            lifespan -= deltaTime;
            

            if (lifespan < 0.0f)
            {
                
                this.Dispose();
            }
            //Move();
        }

        public override void OnBeginPlay()
        {
            Console.WriteLine("Bullet Spawned");
            //rigidbody.ApplyCentralImpulse(initialSpeed);

            //mv_cmp.Jump(initialSpeed);
        }

        public void fire()
        {
            rigidbody.ApplyCentralImpulse(initialSpeed);
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