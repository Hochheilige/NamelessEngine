using Scripts.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public class BTTask_RandomMove : BTTask
    {
        public override object CreateMemoryObject()
        {
            return base.CreateMemoryObject();
        }

        public override TaskStateEnum Execute(BehaviorTreeComponent btComponent, object nodeMemory)
        {
            throw new NotImplementedException();
        }

        public override void TickTask(BehaviorTreeComponent btComponent, object nodeMemory, float deltaTime)
        {
            
            base.TickTask(btComponent, nodeMemory, deltaTime);
        }
    }
}
