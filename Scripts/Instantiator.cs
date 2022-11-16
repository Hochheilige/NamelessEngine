using System;
using System.Runtime.CompilerServices;

namespace Scripts
{
    public static class Instantiator
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern IntPtr InstantiateActorInternal();
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void PhysicsSetGravity(IntPtr handle, float gravity);
        
        static IntPtr InstantiateActor()
        {
            return InstantiateActorInternal();
        }
    }
}