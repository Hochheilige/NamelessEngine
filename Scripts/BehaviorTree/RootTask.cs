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
        public override bool run()
        {
            return true;
        }
    }
}
