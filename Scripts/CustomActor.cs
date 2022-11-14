using System;

namespace Scripts
{
    public class CustomActor : Actor
    {
        public CustomActor()
        {
            Console.WriteLine("Custom actor been born");
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
            //Console.WriteLine("BeginPlay CustomActor");
            var phys = Components.GetComponent<PhysicsComponent>();
            if (!(phys is null))
            {
                phys.SetMass(5.2f);
            }
            
            //Console.WriteLine(ToString() + " Begin Play");
        }
    }
}