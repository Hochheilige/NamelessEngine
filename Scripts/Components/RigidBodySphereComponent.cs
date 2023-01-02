using Scripts.Engine;

namespace Scripts.Components
{
    public sealed class RigidBodySphereComponent : RigidBodyComponent
    {
        public RigidBodySphereComponent(Actor owner, bool internalCreate) : base(owner)
        {
            if (internalCreate)
            {
                this.CppInstance = InternalCreateComponent(owner.CppInstance, (int)ComponentType.RigidBodySphereType);
            }
        }
    }
}