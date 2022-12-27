using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.Components
{
    internal class StaticMeshRenderer : Component
    {
        public StaticMeshRenderer(Actor owner, bool internalCreate) : base (owner)
        {
            if (internalCreate)
                this.CppInstance = InternalCreateComponent(owner.CppInstance, (int)ComponentsEnum.StaticMeshRendererType);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetMeshPath(IntPtr handle, string meshPath);

        public void SetMeshPath(string meshPath)
        {
            InternalSetMeshPath(CppInstance, meshPath);
        }

    }
}
