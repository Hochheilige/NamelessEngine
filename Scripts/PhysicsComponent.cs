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

        public PhysicsComponent(Actor owner, bool internalCreate) : base(owner)
        {
            if (internalCreate)
            {
                this.CppInstance = InternalCreateComponent(owner.CppInstance, (int)ComponentsEnum.RigidBodySphereType);
            }
        }

        public void SetMass(float mass)
        {
            Console.WriteLine("Setted new Mass");
            PhysicsSetMass(CppInstance, mass);
        }

        public void SetGravity(float gravity)
        {
            PhysicsSetGravity(CppInstance, gravity);
        }
    }
}