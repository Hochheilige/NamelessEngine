using Scripts.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public interface Action : Task
    {
        void TickTask(BehaviorTreeComponent btComponent, float deltaTime);
        TaskStateEnum FinishLatentTask();
    }
}
