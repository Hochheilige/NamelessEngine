﻿using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using Scripts.Components;

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
        }

        /**
         * Here and only here components can be properly added to the Actor.
         * Wouldn't be called upon deserializing.
         */
        private protected virtual void RegisterComponents()
        {
            
        }

        private protected virtual void Init()
        {
            
        }

        public void SetCppInstance(IntPtr obj)
        {
            CppInstance = obj;
        }
        
        public virtual void OnBeginPlay() { }
        
        public virtual void Update(float deltaTime) { }
        
        public Component AddComponent(int componentType)
        {
            return CreateComponent(componentType, true);
        }

        private Component AddExternalComponent(int componentType)
        {
            return CreateComponent(componentType, false);
        }

        private Component CreateComponent(int componentType, bool internalCreate)
        {
            ComponentsEnum type = (ComponentsEnum) componentType;
            //Console.WriteLine("Component type to add " + type);
            
            Component component = default;
            switch (type)
            {
                case ComponentsEnum.AudioComponentType:
                    component = new AudioComponent(this);
                    break;
                case ComponentsEnum.RigidBodyCubeType:
                    component = new RigidBodyCubeComponent(this, internalCreate);
                    break;
                case ComponentsEnum.RigidBodySphereType:
                    component = new RigidBodySphereComponent(this, internalCreate);
                    break;
                case ComponentsEnum.MovementComponentType:
                    component = new MovementComponent(this, internalCreate);
                    break;
                case ComponentsEnum.CameraComponentType:
                    component = new CameraComponent(this, internalCreate);
                    break;
                default: 
                    Console.WriteLine("Default");
                    component = new Component(this); break;
            }

            Components.Add(component);
            return component;
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