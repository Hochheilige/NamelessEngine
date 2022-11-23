namespace Scripts.Components
{
    public class RigidBodySphereComponent : RigidBodyComponent
    {
        public RigidBodySphereComponent(Actor owner, bool internalCreate) : base(owner)
        {
            if (internalCreate)
            {
                this.CppInstance = InternalCreateComponent(owner.CppInstance, (int)ComponentsEnum.RigidBodySphereType);
            }
        }
    }
}