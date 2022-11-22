using SharpDX;
using System.Runtime.CompilerServices;

namespace Scripts
{
    public class AudioComponent : Component
    {
        public AudioComponent(Actor owner) : base(owner)
        {
            /*this.Transform = new Transform()
            {
                Position = new Vector3(1, 1, 1),
                Rotation = new Vector4(2, 2, 2, 2),
                Scale = new Vector3(3, 3, 3)
            };*/
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void InternalOnRegister();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern float InternalGetParentTransform();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void InternalOnUnregister();
    }
}