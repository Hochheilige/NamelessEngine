using System;
using System.Runtime.CompilerServices;

namespace Scripts
{
    public class PhysicsComponent : Component
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void PhysicsSetMass(IntPtr handle, float mass);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void PhysicsSetGravity(IntPtr handle, float gravity);

        public PhysicsComponent(Actor owner) : base(owner) { }

        public void SetMass(float mass)
        {
            PhysicsSetMass(CppInstance, mass);
        }

        public void SetGravity(float gravity)
        {
            PhysicsSetGravity(CppInstance, gravity);
        }
    }
}