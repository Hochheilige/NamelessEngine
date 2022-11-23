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
            //AddComponent(new RigidBodyCubeComponent(this));
        }
        
        public override void Update(float deltaTime)
        {
            _transform = GetTransform();
            
            _angle += 0.01f;
            _transform.Rotation = Quaternion.RotationYawPitchRoll(0, 0, _angle);
            _transform.Rotation.Normalize();
            
            SetTransform(_transform);
        }

        private void ReadInput(Keys key)
        {
            //Console.WriteLine("Pressed " + key);
            _transform = GetTransform();
            if (key == Keys.W)
            {
                _transform.Position.Z += 1f;
            }
            
            if (key == Keys.S)
            {
                _transform.Position.Z -= 1f;
            }
            
            SetTransform(_transform);
        }

        public override void OnBeginPlay()
        {
            _transform = GetTransform();

            var physics = Components.GetComponent<RigidBodyCubeComponent>();
 
            if (physics != null)
            {
                //physics.SetLinearVelocity(new Vector3(2f, 2f, 2f));
                physics.SetGravity(5f);
            }
            
            InputHandler.onKeyInput += ReadInput;
        }
    }
}