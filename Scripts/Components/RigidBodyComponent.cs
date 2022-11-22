using System;
using System.Runtime.CompilerServices;

namespace Scripts.Components
{
    public class RigidBodyComponent: Component
    {
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void RigidBodyCubeSetMass(IntPtr handle, float mass);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void RigidBodyCubeSetGravity(IntPtr handle, float gravity);

        public RigidBodyComponent(Actor owner) : base(owner)
        {
            this.CppInstance = InternalCreateComponent(owner.CppInstance, (int) ComponentsEnum.RigidBodyCubeType);
        }

        public void SetMass(float mass)
        {
            RigidBodyCubeSetMass(CppInstance, mass);
        }

        public void SetGravity(float gravity)
        {
            RigidBodyCubeSetGravity(CppInstance, gravity);
        }
    }
}