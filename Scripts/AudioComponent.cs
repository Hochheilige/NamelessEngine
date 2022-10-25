using SharpDX;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Scripts
{
    public class AudioComponent : IComponent
    {
        Transform transform;
        GameObject parent;

        public AudioComponent(GameObject parent)
        {
            this.parent = parent;
            transform = new Transform()
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

        public Transform GetParentTransform()
        {
            return parent.Transform;
        }

        public void OnRegister()
        {
            InternalOnRegister();
        }

        public void OnUnregister()
        {
            InternalOnUnregister();
        }

        public void Update(float deltaTime)
        {
            parent.Transform.Position.X += 1f;
            parent.Transform.Rotation.X += 1f;
            parent.Transform.Scale.X += 1f;
        }
    }
}