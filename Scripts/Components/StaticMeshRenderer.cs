using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using Scripts.Engine;

namespace Scripts.Components
{
    internal sealed class StaticMeshRenderer : Component
    {
        public StaticMeshRenderer(Actor owner, bool internalCreate) : base (owner)
        {
            if (internalCreate)
                this.CppInstance = InternalCreateComponent(owner.CppInstance, (int)ComponentType.StaticMeshRendererType);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetMeshPath(IntPtr handle, string meshPath);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetTexturePath(IntPtr handle, string texturePath);

        public void SetMeshPath(string meshPath)
        {
            InternalSetMeshPath(CppInstance, meshPath);
        }

        public void SetTexturePath(string texturePath)
        {
            InternalSetTexturePath(CppInstance, texturePath);
        }

    }
}
