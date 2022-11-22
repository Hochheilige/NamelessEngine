using System;
using System.Runtime.CompilerServices;

namespace Scripts.Components
{
    public class RigidBodySphereComponent : Component
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void RigidBodySphereSetMass(IntPtr handle, float mass);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void RigidBodySphereSetGravity(IntPtr handle, float gravity);

        public RigidBodySphereComponent(Actor owner) : base(owner)
        {
            this.CppInstance = InternalCreateComponent(owner.CppInstance, (int) ComponentsEnum.RigidBodySphereType);
        }

        public void SetMass(float mass)
        {
            RigidBodySphereSetMass(CppInstance, mass);
        }

        public void SetGravity(float gravity)
        {
            RigidBodySphereSetGravity(CppInstance, gravity);
        }
    }
}