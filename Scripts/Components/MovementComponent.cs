using System.Runtime.CompilerServices;
using System;
using Scripts.Engine;
using SharpDX;

namespace Scripts.Components
{
    public sealed class MovementComponent : Component
    {
        public MovementComponent(Actor owner, bool internalCreate) : base(owner)
        {
            if (internalCreate)
            {
                this.CppInstance =
                    InternalCreateComponent(owner.CppInstance, (int) ComponentType.MovementComponentType);
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalJump(IntPtr handle, Vector3 direction);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetLinearVelocity(IntPtr handle, Vector3 speed);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetWalkDirection(IntPtr handle, Vector3 direction);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetGravity(IntPtr handle, Vector3 gravity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool InternalCanJump(IntPtr handle);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Vector3 InternalGetLinearVelocity(IntPtr handle);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool InternalMoveTo(IntPtr handle, Vector3 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalStopMoveTo(IntPtr handle);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool InternalIsFollowingNavPath(IntPtr handle);

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

        public void SetGravity(Vector3 gravity)
        {
            InternalSetGravity(CppInstance, gravity);
        }

        public bool CanJump()
        {
            return InternalCanJump(CppInstance);
        }

        public Vector3 GetLinearVelocity()
        {
            return InternalGetLinearVelocity(CppInstance);
        }

        public bool MoveTo(Vector3 position)
        {
            return InternalMoveTo(CppInstance, position);
        }

        public void StopMoveTo()
        {
            InternalStopMoveTo(CppInstance);
        }

        public bool IsFollowingNavPath()
        {
            return InternalIsFollowingNavPath(CppInstance);
        }
    }
}
