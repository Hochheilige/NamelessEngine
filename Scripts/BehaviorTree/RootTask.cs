using Scripts.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public class RootTask : Task
    {
        List<Task> tasks = new List<Task>();

        Task child;

        public TaskStateEnum Execute(BehaviorTreeComponent btComponent, object nodeMemory)
        {
            foreach (var task in tasks)
            {
                task.Execute(btComponent, nodeMemory);
            }

            return TaskStateEnum.Succeded ;
        }


    }
}
