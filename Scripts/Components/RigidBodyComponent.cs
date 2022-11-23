using System;
using System.Runtime.CompilerServices;
using SharpDX;

namespace Scripts.Components
{
    public class RigidBodyComponent: Component
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetMass(IntPtr handle, float mass);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetGravity(IntPtr handle, float gravity);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetLinearVelocity(IntPtr handle, Vector3 velocity);

        public RigidBodyComponent(Actor owner, bool internalCreate = true) : base(owner)
        {
        }

        public void SetMass(float mass)
        {
            InternalSetMass(CppInstance, mass);
        }

        public void SetGravity(float gravity)
        {
            InternalSetGravity(CppInstance, gravity);
        }
        
        public void SetLinearVelocity(Vector3 velocity)
        {
            InternalSetLinearVelocity(CppInstance, velocity);
            Console.WriteLine("Velocity worked");
        }
    }
}