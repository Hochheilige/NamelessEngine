using System.Runtime.CompilerServices;
using System;
using SharpDX;

namespace Scripts.Components
{
    public class CameraComponent : Component
    {
        public CameraComponent(Actor owner) : base(owner)
        {
            this.CppInstance = InternalCreateComponent(owner.CppInstance, (int)ComponentsEnum.CameraComponentType);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetCameraTransformInternal(IntPtr handle, Vector3 position, Quaternion rotationQuaternion);

        public void SetCameraTransform(Vector3 position, Quaternion rotationQuaternion)
        {
            SetCameraTransformInternal(CppInstance, position, rotationQuaternion);
        }

    }
}
