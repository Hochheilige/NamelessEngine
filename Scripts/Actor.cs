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
            Console.WriteLine("Component type to add " + type.ToString());
           
            Component component = default;
            switch (type)
            {
                case ComponentsEnum.MeshRenderer:
                    component = new AudioComponent(this);
                    break;
                case ComponentsEnum.RigidBodyCube:
                    component = new PhysicsComponent(this);
                    break;
                case ComponentsEnum.RigidBodySphere:
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