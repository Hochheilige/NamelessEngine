using Scripts.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public abstract class Condition : Task
    {
        public TaskStateEnum Execute()
        {
            throw new NotImplementedException();
        }

        public TaskStateEnum Execute(BehaviorTreeComponent btComponent, object nodeMemory)
        {
            throw new NotImplementedException();
        }
    }
}
