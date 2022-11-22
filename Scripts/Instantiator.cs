using System;
using System.Linq;
using System.Runtime.CompilerServices;

namespace Scripts
{
    public static class Instantiator
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Actor InstantiateActorInternal(string name);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private static extern void PhysicsSetGravity(IntPtr handle, float gravity);
        
        public static Actor InstantiateActor<T>() where T : Actor
        {
            var result = typeof(T).ToString().Split('.').Last();
            Console.WriteLine("Actor " + result + "Was Called to Instantiate");
            return InstantiateActorInternal(result);
        }
    }
}