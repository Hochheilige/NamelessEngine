using Scripts.Engine;

namespace Scripts.Components
{
    public sealed class RigidBodyCubeComponent : RigidBodyComponent
    {
        public RigidBodyCubeComponent(Actor owner, bool internalCreate = true) : base(owner)
        {
            if (internalCreate)
            {
                this.CppInstance = InternalCreateComponent(owner.CppInstance, (int)ComponentType.RigidBodyCubeType);
            }
        }
    }
}