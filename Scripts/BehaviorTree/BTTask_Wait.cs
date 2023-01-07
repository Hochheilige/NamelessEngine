using Scripts.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public class BTTask_Wait : BTTask
    {

        public BTTask_Wait() { }

        public BTTask_Wait(float waitTime)
        {
            this.waitTime = waitTime;
        }

        private float waitTime = 5.0f;

        public override object CreateMemoryObject()
        {
            return new BTWait_Task_Memory();
        }

        public override TaskStateEnum Execute(BehaviorTreeComponent btComponent, object nodeMemory)
        {
            (nodeMemory as BTWait_Task_Memory).timeRemaining = waitTime;

            return TaskStateEnum.InProgress;
        }

        public override void TickTask(BehaviorTreeComponent btComponent, object nodeMemory, float deltaTime)
        {
            BTWait_Task_Memory waitMem = nodeMemory as BTWait_Task_Memory;

            waitMem.timeRemaining -= deltaTime;

            if (waitMem.timeRemaining < 0.0f)
            {
                FinishLatentTask(btComponent, TaskStateEnum.Succeded);
            }
        }
    }

    public class BTWait_Task_Memory
    {
        public float timeRemaining;
    }
}
