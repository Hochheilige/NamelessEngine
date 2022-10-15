using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Scripts
{
    public class AudioComponent //: IComponent
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void InternalOnRegister();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern float InternalGetParentTransform();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void InternalOnUnregister();

        public float GetParentTransform()
        {
            return 10.0f;
        }

        public static void OnRegister()
        {
            InternalOnRegister();
        }

        public void OnUnregister()
        {
            InternalOnUnregister();
        }

        public void Update()
        {
            throw new NotImplementedException();
        }
    }
}
