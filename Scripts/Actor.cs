using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace Scripts
{
    public class Actor : IDisposable
    {
        public IntPtr CppInstance;
        public List<Component> Components = new List<Component>();
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalAddComponent(int componentType);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Transform InternalGetTransform(IntPtr cppInstance);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetTransform(IntPtr cppInstance, Transform transform);

        public Actor()
        {
            Console.WriteLine("Base CTOR");
        }

        public void SetCppInstance(IntPtr obj)
        {
            Console.WriteLine("setted");
            CppInstance = obj;
        }
        
        public virtual void OnBeginPlay() { }
        
        public virtual void Update(float deltaTime) { }
        
        public Component AddComponent(int componentType)
        {
            ComponentsEnum type = (ComponentsEnum) componentType;
            //Console.WriteLine("Component type to add " + type);
            
            Component component = default;
            switch (type)
            {
                case ComponentsEnum.MeshRendererType:
                    component = new AudioComponent(this);
                    break;
                case ComponentsEnum.RigidBodyCubeType:
                    component = new PhysicsComponent(this);
                    break;
                case ComponentsEnum.RigidBodySphereType:
                    component = new PhysicsComponent(this);
                    break;
                default: component = new Component(this); break;
            }

            AddComponent(component);
            return component;
        }

        public void AddComponent(Component component)
        {
            Components.Add(component);
        }

        public Transform GetTransform()
        {
            return InternalGetTransform(CppInstance);
        }

        public void SetTransform(Transform transform)
        {
            InternalSetTransform(CppInstance, transform);
        }

        public void Dispose()
        {
            //Components.ForEach(x => x.Dispose());
        }
    }
}