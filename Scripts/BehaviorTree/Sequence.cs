using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public class Sequence : Task
    {
        List<Task> tasks = new List<Task>();

        public void AddTask(Task task)
        {
            tasks.Add(task);
        }
        public override bool run()
        {
            foreach (var task in tasks)
            {
                if (!task.run())
                {
                    return false;
                }
            }
            return true;
        }
    }
}
