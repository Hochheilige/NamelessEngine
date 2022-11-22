using System;
using Scripts.Components;

namespace Scripts
{
    public class Bullet : Actor
    {
        public Bullet()
        {
            AddComponent(new RigidBodyCubeComponent(this));
        }
        
        public override void Update(float deltaTime)
        {
            Move();
        }

        public override void OnBeginPlay()
        {
            Console.WriteLine("Bullet Spawned");
        }

        private void Move()
        {
            var root = GetTransform();
            root.Position.X += 0.0001f;
            root.Position.X += 0.0001f;
            root.Position.X += 0.0001f;
            SetTransform(root);
        }
    }
}