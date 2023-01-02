using Scripts.Components;

namespace Scripts.Tests
{
    internal class SimpleProjectile : Actor
    {
        private readonly MovementComponent mv_cmp;
        
        SimpleProjectile()
        {
            mv_cmp = AddComponent<MovementComponent>();
        }

        private float lifespan = 10.0f;

    }
}
