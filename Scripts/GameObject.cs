using System;
using System.Collections.Generic;
using SharpDX;

namespace Scripts
{
    public abstract class GameObject 
    {
        public Transform Transform;

        public List<IComponent> Components { get; set; } = new List<IComponent> ();

        public GameObject() { }

        public GameObject(Transform transform, List<IComponent> components)
        {
            Transform = transform;
            Components = components;
        }

        public GameObject(Transform transform)
        {
            Transform = transform;
        }

        public void AddComponent(IComponent component)
        {
            Components.Add(component);
        }
       

        public virtual void Update(float time)
        {
            Console.WriteLine("TEst");
            /*foreach (IComponent component in Components)
            {
                component.Update(deltaTime);
            }*/
        }

        public abstract void OnDestroy();

        public virtual void OnBeginPlay()
        {

        }
    }
}