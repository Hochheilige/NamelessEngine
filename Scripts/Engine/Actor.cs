using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using Scripts.Components;
using Scripts.Engine;
using Scripts.Extensions;

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
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDestroy(IntPtr cppInstance);

        public Actor()
        {
        }

        /**
         * Here and only here components can be properly added to the Actor.
         * Wouldn't be called upon deserializing.
         */
        protected internal virtual void RegisterComponents()
        {
            
        }

        protected internal virtual void Init()
        {
            
        }

        public void SetCppInstance(IntPtr obj)
        {
            CppInstance = obj;
            Game.GetInstance().AddActor(this);
        }
        
        public virtual void OnBeginPlay() { }
        
        public virtual void Update(float deltaTime) { }

        protected internal virtual void OnKeyInput(Keys key, InputHandler.KeyAction type)
        {

        }
        
        protected internal virtual void OnMouseInput(InputHandler.MouseButton button, InputHandler.MouseAction action)
        {
            
        }
        
        public Component AddComponent(ComponentType componentType)
        {
            return CreateComponent((int)componentType, true);
        }

        protected T AddComponent<T>(string name = null) where T : Component
        {
            return CreateComponent((int)ComponentHelper.GetEnumValueFromType<T>(), true, name) as T;
        }

        private Component AddExternalComponent(int componentType)
        {
            return CreateComponent(componentType, false);
        }

        private Component CreateComponent(int componentType, bool internalCreate, string name = null)
        {
            ComponentType type = (ComponentType) componentType;

            if (internalCreate && name != null)
            {
                var intComp = Components.FirstOrDefault(x => x.GetName().Equals(name));
                if (intComp != null)
                {
                    return intComp;
                }
            }

            Component component = default;
            switch (type)
            {
                case ComponentType.RigidBodyCubeType:
                    component = new RigidBodyCubeComponent(this, internalCreate);
                    break;
                case ComponentType.RigidBodySphereType:
                    component = new RigidBodySphereComponent(this, internalCreate);
                    break;
                case ComponentType.MovementComponentType:
                    component = new MovementComponent(this, internalCreate);
                    break;
                case ComponentType.CameraComponentType:
                    component = new CameraComponent(this, internalCreate);
                    break;
                case ComponentType.StaticMeshRendererType:
                    component = new StaticMeshRenderer(this, internalCreate);
                    break;
                case ComponentType.AudioComponentType:
                    component = new AudioComponent(this, internalCreate);
                    break;
                default: 
                    Console.WriteLine("Default");
                    throw new Exception("Undefined Component");
            }

            if (internalCreate)
            {
                component.SetName(name ?? component.GetIdentifier());
                //component.Name = name ?? component.GetHashCode().ToString();
            }

            Components.Add(component);
            return component;
        }

        public Transform GetTransform()
        {
            return InternalGetTransform(CppInstance);
        }

        public virtual void SetTransform(Transform transform)
        {
            InternalSetTransform(CppInstance, transform);
        }

        public void Dispose()
        {
            Game.GetInstance().RemoveActor(this);
            //Components.ForEach(x => x.Dispose());
        }

        protected virtual void Overlap(Actor otherActor) { }

        internal virtual void OnGUI()
        {
            Console.WriteLine("Base");
        }

        internal void Destroy()
        {
            InternalDestroy(CppInstance);
        }
        
        private void cpp_RegisterComponents()
        {
            RegisterComponents();
        }
        
        private void cpp_Init()
        {
            Init();
        }
    }
}