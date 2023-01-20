using System;
using System.Runtime.CompilerServices;
using Scripts.Engine;

namespace Scripts
{
    public abstract class Component : IDisposable
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        protected static extern IntPtr InternalCreateComponent(IntPtr actor, int componentType);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        protected static extern void InternalSetName(IntPtr actor, String name);

        
        [MethodImpl(MethodImplOptions.InternalCall)]
        protected static extern string InternalGetName(IntPtr actor);

        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Transform InternalGetTransform(IntPtr handle);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetTransform(IntPtr handle, Transform transform);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetRelativeTransform(IntPtr handle, Transform transform);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Transform InternalGetRelativeTransform(IntPtr handle);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Transform InternalSetAttachmentParent(IntPtr handle, IntPtr parent);

        private Actor Owner { get; set; }
        
        public Transform Transform;
        protected IntPtr CppInstance;
        public string Name;

        protected Component(Actor owner)
        {
            Owner = owner;
        }

        private void SetCppInstance(IntPtr obj)
        {
            CppInstance = obj;
        }

        public void SetName(string name)
        {
            InternalSetName(CppInstance, name);
            this.Name = name;
        }

        public string GetName()
        {
            return InternalGetName(CppInstance);
        }

        public Transform GetTransform()
        {
            return InternalGetTransform(CppInstance);
        }

        public void SetTransform(Transform transform)
        {
            InternalSetTransform(CppInstance, transform);
        }

        public void SetRelativeTransform(Transform transform)
        {
            InternalSetRelativeTransform(CppInstance, transform);
        }

        public Transform GetRelativeTransform()
        {
            return InternalGetRelativeTransform(CppInstance);
        }

        public void Dispose()
        {
            CppInstance = IntPtr.Zero;
            Owner.Components.Remove(this);
        }

        public string GetIdentifier()
        {
            return CppInstance.ToString();
        }

        public void SetAttachmentParent(Component parent)
        {
            InternalSetAttachmentParent(CppInstance, parent.CppInstance);
        }
    }
}