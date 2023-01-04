using Scripts.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public abstract class Composite : Task
    {
        Task currenNode = null;
        public abstract TaskStateEnum Execute(BehaviorTreeComponent btComponent, object nodeMemory);
    }
}
