using System.Runtime.CompilerServices;

namespace Scripts
{
    public class PhysicsComponent : Component
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void InternalOnUnregister();
        
        public override void OnRegister()
        {
            throw new System.NotImplementedException();
        }

        public override void OnUnregister()
        {
            throw new System.NotImplementedException();
        }

        public override void Update(float deltaTime)
        {
            throw new System.NotImplementedException();
        }

        public PhysicsComponent(Transform transform, GameObject parent) : base(transform, parent)
        {
        }

        public PhysicsComponent(GameObject parent) : base(parent)
        {
        }
    }
}