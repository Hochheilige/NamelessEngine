namespace Scripts.Components
{
    public class MeshRendererComponent : Component
    {
        public MeshRendererComponent(Actor owner) : base(owner)
        {
            this.CppInstance = InternalCreateComponent(owner.CppInstance, (int)ComponentsEnum.MovementComponentType);
        }
    }
}