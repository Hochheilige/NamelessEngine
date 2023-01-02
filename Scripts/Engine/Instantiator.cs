using System;
using System.Linq;
using System.Runtime.CompilerServices;

namespace Scripts.Engine
{
    public static class Instantiator
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Actor InstantiateActorInternal(string name);

        public static T InstantiateActor<T>() where T : Actor
        {
            var result = typeof(T).ToString().Split('.').Last();
            Console.WriteLine("Actor " + result + " Was Called to Instantiate");
            return InstantiateActorInternal(result) as T;
        }
    }
}