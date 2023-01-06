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

        MeshRendererComponent meshRenderer;
        BehaviorTreeComponent btComp;
        private protected override void Init()
        {
            meshRenderer = AddComponent<MeshRendererComponent>("Mesh Renderer");
            btComp = new BehaviorTreeComponent(this);
        }

        public override void Update(float deltaTime)
        {

        }
    }
}
