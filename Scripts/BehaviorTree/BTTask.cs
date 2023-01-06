using Scripts.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public abstract class BTTask : BTNode
    {
        public virtual object CreateMemoryObject() { return null; }
        public abstract TaskStateEnum Execute(BehaviorTreeComponent btComponent, object nodeMemory);

        public virtual void TickTask(BehaviorTreeComponent btComponent, object nodeMemory, float deltaTime) { }

        protected void FinishLatentTask(BehaviorTreeComponent btComponent, TaskStateEnum result)
        {
            btComponent.FinishLatentTask(this, result);
        }
    }
}
