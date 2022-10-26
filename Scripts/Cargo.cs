using SharpDX;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts
{
    public class Cargo : GameObject
    {
        public Cargo()
        {
            Transform = new Transform()
            {
                Position = new Vector3(1, 1, 1),
                Rotation = new Vector4(2, 2, 2, 2),
                Scale = new Vector3(3, 3, 3)
            };

            OnBeginPlay();
        }

        public Cargo(Transform transform) : base(transform)
        {
            OnBeginPlay();
        }

        public Cargo(Transform transform, List<Component> components) : base(transform, components)
        {
        }

        public override void OnBeginPlay()
        {
            Console.WriteLine("Begin Play");
            AddComponent(new AudioComponent(this));
            Components.GetComponent<AudioComponent>();
            base.OnBeginPlay();
        }

        public override void Update(float time)
        {
            foreach (Component component in Components)
            {
               component.Update(2);
            }
        }

        public Transform GetTransform()
        {
            var tr = Components.First().GetParentTransform();
            return Components.First().GetParentTransform();
        }
    }
}