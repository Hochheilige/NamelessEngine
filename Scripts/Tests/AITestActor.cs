using Scripts.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Scripts.BehaviorTree;
using SharpDX;

namespace Scripts.Tests
{
    public class AITestActor : Actor
    {

        StaticMeshRenderer meshRenderer;
        BehaviorTreeComponent btComp;

        protected internal override void Init()
        {
            AddComponent<MovementComponent>("MovementComponent");

            meshRenderer = AddComponent<StaticMeshRenderer>("Mesh Renderer");
            meshRenderer.SetMeshPath("../Assets/box.fbx/Cube");

            // Right now we need to create a component manually like this
            btComp = new BehaviorTreeComponent(this);
            btComp.SetBTTree(BehaviorTreeManager.Instance.GetTree("../Assets/AI/BehaviorTree0.json"));

            // we can set a blackboard value like this
            // this values can be used by task nodes (like BTTask_MoveTo)
            // some tasks may add/or update these blackboard values (like BTTask_UpdateBlackboardWithPlayerPosition)
            btComp.blackboard["MoveToPos"] = new Vector3(0.0f, 0.0f, 0.0f);
        }

        public override void Update(float deltaTime)
        {
            // Right now we need to call execute on btComponent maually to make it run the behavior tree
            btComp.Execute(deltaTime);
        }
    }
}
