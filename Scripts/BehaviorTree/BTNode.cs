using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public class BTNode
    {
        public BTComposite GetParent() { return parent; }

        private BTComposite parent;
    }
}
