using System;

namespace Scripts
{
    public class Component : IDisposable
    {
        public Actor Owner { get; set; }
        public Transform Transform;
        public IntPtr CppInstance;

        public Component(Actor owner)
        {
            Owner = owner;
        }

        public void SetCppInstance(IntPtr obj)
        {
            CppInstance = obj;
        }

        public void SetTransform(Transform transform)
        {
            Transform = transform;
        }

        public void Dispose()
        {
            CppInstance = IntPtr.Zero;
        }
    }
}