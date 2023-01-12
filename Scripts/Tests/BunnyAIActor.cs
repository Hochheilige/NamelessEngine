using Scripts.BehaviorTree;
using Scripts.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.Tests
{
    public class BunnyAIActor : Actor
    {

        BehaviorTreeComponent btComp;
        protected internal override void Init()
        {
            AddComponent<StaticMeshRenderer>("Mesh Renderer").SetMeshPath("../Assets/box.fbx/Cube");


            btComp = new BehaviorTreeComponent(this);

            BTTree tree = new BTTree("../Assets/AI/BehaviorTree1.json");
            btComp.SetBTTree(tree);

            //BTSequence sequence = new BTSequence();
            //tree.SetRoot(sequence);
            //BTSelector selector = new BTSelector();
            //sequence.AddChild(selector);
            //BTTask_Wait wait = new BTTask_Wait();
            //sequence.AddChild(wait);
            //BTTask_RandomMove move = new BTTask_RandomMove();
            //sequence.AddChild(move);

            //BTTask_IsFarFromPlayer farFromPlayerTask = new BTTask_IsFarFromPlayer(4.0f);
            //selector.AddChild(farFromPlayerTask);

            //BTTask_MoveAwayFromPlayer moveAwayFromPlayerTask = new BTTask_MoveAwayFromPlayer(moveDistance: 5.0f, moveTime: 0.5f);
            //selector.AddChild(moveAwayFromPlayerTask);
        }

        public override void Update(float deltaTime)
        {
            btComp.GetExecutor().Execute(deltaTime);
        }

    }
}
