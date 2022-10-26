using System;
using System.Collections.Generic;
using SharpDX;

namespace Scripts
{
    public abstract class GameObject 
    {
        public Transform Transform;

        public List<Component> Components { get; set; } = new List<Component> ();

        public GameObject() { }

        public GameObject(Transform transform, List<Component> components)
        {
            Transform = transform;
            Components = components;
        }

        public GameObject(Transform transform)
        {
            Transform = transform;
        }

        public void AddComponent(Component component)
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

        public virtual void OnDestroy()
        {
            
        }

        public virtual void OnBeginPlay()
        {

        }
    }
}