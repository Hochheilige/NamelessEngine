using Scripts.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public abstract class Condition : BTTask
    {
        public TaskStateEnum Execute()
        {
            throw new NotImplementedException();
        }

    }
}
