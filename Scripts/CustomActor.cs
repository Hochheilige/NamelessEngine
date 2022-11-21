using System;

namespace Scripts
{
    public class CustomActor : Actor
    {
        public CustomActor()
        {
            //AddComponent((int)ComponentsEnum.RigidBodyCubeType);
            //Components.Add(new PhysicsComponent());
            //Console.WriteLine("Custom actor been born");
        }
        
        public override void Update(float deltaTime)
        {
            //Console.WriteLine(Components.Count + " Custom Update");
            
            /*var phys = Components.GetComponent<PhysicsComponent>();
            if (!(phys is null))
            {
                phys.SetMass(5.2f);
            }*/
        }

        public override void OnBeginPlay()
        {
            //Components.Add(new PhysicsComponent(this));
            AddComponent((int)ComponentsEnum.RigidBodyCubeType);
            //AddComponent((int)ComponentsEnum.MeshRendererType);
        }
    }
}