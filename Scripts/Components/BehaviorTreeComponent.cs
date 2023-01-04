using Scripts.BehaviorTree;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Scripts.Components
{
    public class BehaviorTreeComponent
    {
        public float deltaTime;

        private List<Task> tasks = new List<Task>();

        private RootTask root;

        public void AddTask(Task task)
        {
            tasks.Add(task);
        }

        public TaskStateEnum Execute()
        {
            return root.Execute(this, null);
        }

    }
}
