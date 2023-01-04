using Scripts.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public class Sequence : Composite
    {
        List<Task> tasks = new List<Task>();

        public void AddTask(Task task)
        {
            tasks.Add(task);
        }

        public override TaskStateEnum Execute(BehaviorTreeComponent btComponent, object nodeMemory)
        {
            throw new NotImplementedException();
        }
    }
}
