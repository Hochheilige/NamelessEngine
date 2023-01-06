using System;
using System.Linq;
using System.Runtime.CompilerServices;

namespace Scripts.Engine
{
    public static class Instantiator
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Actor InstantiateActorInternal(string ns, string name);

        public static T InstantiateActor<T>() where T : Actor
        {
            var str = typeof(T).ToString();
            Console.WriteLine("Actor " + str + " Was Called to Instantiate");
            var dividerIndex = str.LastIndexOf('.');
            var ns = str.Substring(0, str.LastIndexOf('.'));
            var name = str.Substring(dividerIndex + 1, str.Length - dividerIndex - 1);
            return InstantiateActorInternal(ns, name) as T;
        }
    }
}