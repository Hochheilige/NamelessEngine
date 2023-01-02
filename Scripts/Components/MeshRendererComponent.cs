using Scripts.Engine;

namespace Scripts.Components
{
    public sealed class MeshRendererComponent : Component
    {
        public MeshRendererComponent(Actor owner) : base(owner)
        {
            this.CppInstance = InternalCreateComponent(owner.CppInstance, (int)ComponentType.MovementComponentType);
        }
    }
}