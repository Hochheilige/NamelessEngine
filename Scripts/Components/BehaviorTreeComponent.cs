using Scripts.BehaviorTree;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Scripts.Components
{
    // TODO: update it to be like other components
    public class BehaviorTreeComponent
    {
        public BehaviorTreeComponent(Actor inOwner)
        {
            executor = new BTTreeExecutor(this);
            owner = inOwner;
        }

        Actor owner;

        public Actor GetOwner() { return owner; }

        public void SetBTTree(BTTree tree)
        {
            executor.SetTreeToExecute(tree);
        }

        BTTreeExecutor executor;

        public void Execute(float deltaTime) { executor.Execute(deltaTime); }

        public BTTreeExecutor GetExecutor() { return executor; }

        public void FinishLatentTask(BTTask task, TaskStateEnum result)
        {
            executor.StopInProgressTask(task, result);
        }
    }
}
