﻿using System;
using System.Runtime.CompilerServices;
using Scripts.Engine;
using SharpDX;

namespace Scripts.Components
{
    public enum RigidBodyType
    {
        Static = 0,
        Dynamic = 1,
        Kinematic = 2,
    }

    public enum RigidBodyUsage
    {
        Physics = 0,
        Collisions = 1,
        CollisionsAndPhysics = 2,
        NoCollisions = 0,
        OverlapAll = 1,
        BlockAll = 2,
    }

    public enum CollisionShape
    {
        Box,
        Sphere,
        Capsule,
    }

    public class RigidBodyComponent: Component
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetMass(IntPtr handle, float mass);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetGravity(IntPtr handle, float gravity);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetLinearVelocity(IntPtr handle, Vector3 velocity);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalApplyCentralImpulse(IntPtr handle, Vector3 impulse);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalEnablePhysicsSimulation(IntPtr handle);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalDisablePhysicsSimulation(IntPtr handle);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetType(IntPtr handle, RigidBodyType type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetUsage(IntPtr handle, RigidBodyUsage usage);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetCollisionShape(IntPtr handle, CollisionShape shape);     
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetGenerateHitEvents(IntPtr handle, bool generate);        
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetGenerateOverlapEvents(IntPtr handle, bool generate);


        public RigidBodyComponent(Actor owner, bool internalCreate = true) : base(owner) 
        {
            if (internalCreate)
            {
                this.CppInstance = InternalCreateComponent(owner.CppInstance, (int)ComponentType.RigidBodyType);
            }
        }

        public void GenerateHitEvents(bool generate)
        {
            InternalSetGenerateHitEvents(CppInstance, generate);
        }

        public void GenerateOverlapEvents(bool generate)
        {
            InternalSetGenerateOverlapEvents(CppInstance, generate);
        }

        public void SetMass(float mass)
        {
            InternalSetMass(CppInstance, mass);
        }

        public void SetGravity(float gravity)
        {
            InternalSetGravity(CppInstance, gravity);
        }
        
        public void SetLinearVelocity(Vector3 velocity)
        {
            InternalSetLinearVelocity(CppInstance, velocity);
            //Console.WriteLine("Velocity worked");
        }

        public void ApplyCentralImpulse(Vector3 impulse)
        {
            InternalApplyCentralImpulse(CppInstance, impulse);
        }

        public void EnablePhysicsSimulation()
        {
            InternalEnablePhysicsSimulation(CppInstance);
        }

        public void DisablePhysicsSimulation()
        {
            InternalDisablePhysicsSimulation(CppInstance);
        }

        public void SetType(RigidBodyType type)
        {
            InternalSetType(CppInstance, type);
        }

        public void SetUsage(RigidBodyUsage usage)
        {
            InternalSetUsage(CppInstance, usage);
        }

        public void SetCollisionShape(CollisionShape shape)
        {
            InternalSetCollisionShape(CppInstance, shape);
        }
    }
}