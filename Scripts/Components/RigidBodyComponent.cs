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
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalApplyCentralImpulse(IntPtr handle, Vector3 impulse);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalEnablePhysicsSimulation(IntPtr handle);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDisablePhysicsSimulation(IntPtr handle);


        protected RigidBodyComponent(Actor owner, bool internalCreate = true) : base(owner) { }

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
            //Console.WriteLine("Velocity worked");
        }

        public void ApplyCentralImpulse(Vector3 impulse)
        {
            InternalApplyCentralImpulse(CppInstance, impulse);
        }

        public void EnablePhysicsSimulation()
        {
            InternalEnablePhysicsSimulation(CppInstance);
        }

        public void DisablePhysicsSimulation()
        {
            InternalDisablePhysicsSimulation(CppInstance);
        }

    }
}