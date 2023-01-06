using Scripts.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Scripts.BehaviorTree;

namespace Scripts.Tests
{
    public class AITestActor : Actor
    {

        StaticMeshRenderer meshRenderer;
        BehaviorTreeComponent btComp;
        protected internal override void Init()
        {
            meshRenderer = AddComponent<StaticMeshRenderer>("Mesh Renderer");
            meshRenderer.SetMeshPath("../Assets/box.fbx/Cube");
            btComp = new BehaviorTreeComponent(this);

            BTTree tree = new BTTree();
            BTSequence sequence = new BTSequence();
            tree.SetRoot(sequence);
            BTTask_Wait wait = new BTTask_Wait();
            sequence.AddChild(wait);
            BTTask_RandomMove move = new BTTask_RandomMove();
            sequence.AddChild(move);
            btComp.SetBTTree(tree);
        }

        public override void Update(float deltaTime)
        {
            btComp.GetExecutor().Execute(deltaTime);
        }
    }
}
