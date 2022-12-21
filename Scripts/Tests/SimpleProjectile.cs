using Scripts.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.Tests
{
    internal class SimpleProjectile : Actor
    {
        private MovementComponent mv_cmp;
        SimpleProjectile()
        {
            mv_cmp = (MovementComponent)AddComponent((int)ComponentsEnum.MovementComponentType);
        }

        private float lifespan = 10.0f;

    }
}
