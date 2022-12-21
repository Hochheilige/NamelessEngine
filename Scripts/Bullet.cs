using System;
using Scripts.Components;
using SharpDX;

namespace Scripts
{
    public class Bullet : Actor
    {

        private StaticMeshRenderer renderer;
        private MovementComponent mv_cmp;
        private float lifespan = 4.0f;
        private Vector3 initialSpeed = new Vector3(10.0f, 10.0f, 10.0f);
        public Bullet()
        {
            AddComponent(mv_cmp = new MovementComponent(this));
            AddComponent(renderer = new StaticMeshRenderer(this));
            renderer.SetMeshPath("../Assets/box.fbx/Cube");
        }
        
        public override void Update(float deltaTime)
        {
            lifespan -= deltaTime;
            
            //Move();
        }

        public override void OnBeginPlay()
        {
            Console.WriteLine("Bullet Spawned");
            mv_cmp.Jump(initialSpeed);
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