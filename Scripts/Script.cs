using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;

namespace Scripts
{
    public class Script
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern void CreateCubeObject(float trans_x, float trans_y, float trans_z,
        float rot_x, float rot_y, float rot_z,
        float scale_x, float scale_y, float scale_z);
        static void Create()
        {
            CreateCubeObject(1, 1, 1, 1, 1, 1, 1, 1, 1);
        }
    }
}
