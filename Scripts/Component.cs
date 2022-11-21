using System;
using System.Runtime.CompilerServices;

namespace Scripts
{
    public class Component : IDisposable
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr InternalCreateComponent(IntPtr actor, int componentType);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Transform InternalGetTransform(IntPtr handle);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetTransform(IntPtr handle, Transform transform);

        public Actor Owner { get; set; }
        public Transform Transform;
        public IntPtr CppInstance;

        public Component(Actor owner)
        {
            Owner = owner;
        }

        public void SetCppInstance(IntPtr obj)
        {
            //Console.WriteLine("Instance was set");
            CppInstance = obj;
            //Transform = InternalGetTransform(CppInstance);
        }

        public void SetTransform(Transform transform)
        {
            //Transform = InternalSetTransform(CppInstance, transform);
        }

        public void Dispose()
        {
            CppInstance = IntPtr.Zero;
        }
    }
}