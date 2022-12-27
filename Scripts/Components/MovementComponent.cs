using System.Runtime.CompilerServices;
using System;
using SharpDX;

namespace Scripts.Components
{
    public class MovementComponent : Component
    {
        public MovementComponent(Actor owner, bool internalCreate) : base(owner)
        {
            if (internalCreate)
            {
                this.CppInstance =
                    InternalCreateComponent(owner.CppInstance, (int) ComponentsEnum.MovementComponentType);
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalJump(IntPtr handle, Vector3 direction);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetLinearVelocity(IntPtr handle, Vector3 speed);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetWalkDirection(IntPtr handle, Vector3 direction);

        public void Jump(Vector3 direction)
        {
            InternalJump(CppInstance, direction);
        }

        public void SetLinearVelocity(Vector3 speed)
        {
            InternalSetLinearVelocity(CppInstance, speed);
        }

        public void SetWalkDirection(Vector3 direction)
        {
            InternalSetWalkDirection(CppInstance, direction);
        }

    }
}
