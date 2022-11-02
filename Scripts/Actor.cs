using System;
using System.Collections.Generic;

namespace Scripts
{
    public class Actor : IDisposable
    {
        public List<Component> Components = new List<Component>();

        public Actor()
        {
            Console.WriteLine("YA RODILSYA");
        }
        
        public Component AddComponent(int componentType)
        {
            ComponentsEnum type = (ComponentsEnum) componentType;
            Component component = default;
            switch (type)
            {
                case ComponentsEnum.AudioComponent:
                    component = new AudioComponent(this);
                    break;
                case ComponentsEnum.PhysicsComponent:
                    component = new PhysicsComponent(this);
                    break;
            }
            
            Components.Add(component);
            return component;
        }

        public void Dispose()
        {
            Components.ForEach(x => x.Dispose());
            Console.WriteLine("YA UMER");
        }
    }
}