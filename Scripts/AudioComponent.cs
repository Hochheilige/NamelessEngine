using SharpDX;
using System.Runtime.CompilerServices;

namespace Scripts
{
    public class AudioComponent : Component
    {
        public AudioComponent(GameObject parent) : base(parent)
        {
            this.Transform = new Transform()
            {
                Position = new Vector3(1, 1, 1),
                Rotation = new Vector4(2, 2, 2, 2),
                Scale = new Vector3(3, 3, 3)
            };

            OnRegister();        
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void InternalOnRegister();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern float InternalGetParentTransform();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void InternalOnUnregister();

        public sealed override void OnRegister()
        {
            InternalOnRegister();
        }

        public override void OnUnregister()
        {
            InternalOnUnregister();
        }

        public override void Update(float deltaTime)
        {
            Parent.Transform.Position.X += 1f;
            Parent.Transform.Rotation.X += 1f;
            Parent.Transform.Scale.X += 1f;
        }
    }
}