using System;
using Scripts.Components;
using SharpDX;

namespace Scripts
{
    public class CustomActor : Actor
    {
        private Transform _transform;
        private float _angle = 0.01f;

        public CustomActor()
        {
            AddComponent(new RigidBodyCubeComponent(this));
        }
        
        public override void Update(float deltaTime)
        {
            _angle += 0.01f;
            _transform.Rotation = Quaternion.RotationYawPitchRoll(0, 0, _angle);
            _transform.Rotation.Normalize();
            
            SetTransform(_transform);
        }

        public override void OnBeginPlay()
        {
            _transform = GetTransform();
            Instantiator.InstantiateActor<Bullet>();
        }
    }
}