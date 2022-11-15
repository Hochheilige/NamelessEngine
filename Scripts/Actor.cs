using System;
using System.Collections.Generic;
using System.Linq;

namespace Scripts
{
    public class Actor : IDisposable
    {
        public List<Component> Components = new List<Component>();

        public Actor()
        {
            //Console.WriteLine("YA RODILSYA");
        }

        public virtual void OnBeginPlay()
        {
            
        }
        
        public virtual void Update(float deltaTime)
        {
            Console.WriteLine("Base Update");
        }
        
        public virtual Component AddComponent(int componentType)
        {
            ComponentsEnum type = (ComponentsEnum) componentType;
            Console.WriteLine("Component type to add " + type.ToString());
           
            Component component = default;
            switch (type)
            {
                case ComponentsEnum.MeshRendererType:
                    component = new AudioComponent(this);
                    break;
                case ComponentsEnum.RigidBodyCubeType:
                    component = new PhysicsComponent(this);
                    break;
                case ComponentsEnum.RigidBodySphereType:
                    component = new PhysicsComponent(this);
                    break;
                default: component = new Component(this); break;
            }
            
            Components.Add(component);
            return component;
        }

        /*public virtual string GetInheritors()
        {
            var listOfBs = (
                    from domainAssembly in AppDomain.CurrentDomain.GetAssemblies()
                    from type in domainAssembly.GetTypes()
                    where typeof(Actor).IsAssignableFrom(type)
                    select type.ToString()).ToArray();
            
            var result = string.Join("\n", listOfBs);
            //Console.WriteLine(result);
            return result;
        }*/

        public void Dispose()
        {
            Components.ForEach(x => x.Dispose());
            Console.WriteLine("YA UMER");
        }
    }
}