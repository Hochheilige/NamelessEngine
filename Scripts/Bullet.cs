using System.Linq;

namespace Scripts
{
    public class Bullet : Actor
    {
        public override void Update(float deltaTime)
        {
            Move();
        }

        private void Move()
        {
            var root = Components.First();
            var newTransform = root.Transform;
            root.SetTransform(newTransform);
        }
    }
}