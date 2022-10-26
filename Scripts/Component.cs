using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;

namespace Scripts
{
    public abstract class Component
    {
        private protected Transform Transform;
        private protected GameObject Parent;
        
        public Component(Transform transform, GameObject parent)
        {
            Transform = transform;
            Parent = parent;
        }

        public Component(GameObject parent)
        {
            Parent = parent;
            Transform = new Transform()
            {
                Position = new Vector3(1, 1, 1),
                Rotation = new Vector4(1, 1, 1, 1),
                Scale = new Vector3(1, 1, 1)
            };
        }
        
        public abstract void OnRegister();

        public abstract void OnUnregister();

        public Transform GetParentTransform()
        {
            return Parent.Transform;
        }
        
        public Transform GetTransform()
        {
            return Parent.Transform;
        }

        public GameObject GetParent()
        {
            return Parent;
        }

        public abstract void Update(float deltaTime);
    }
}