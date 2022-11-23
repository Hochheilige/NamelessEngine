namespace Scripts.Components
{
    public class RigidBodyCubeComponent : RigidBodyComponent
    {
        public RigidBodyCubeComponent(Actor owner, bool internalCreate = true) : base(owner)
        {
            if (internalCreate)
            {
                this.CppInstance = InternalCreateComponent(owner.CppInstance, (int)ComponentsEnum.RigidBodyCubeType);
            }
        }
    }
}