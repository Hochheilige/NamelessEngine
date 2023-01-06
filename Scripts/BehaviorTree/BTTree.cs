using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public class BTTree
    {
        // not the root node per se, but the only child that a root node might have
        private BTComposite root = null;

        public BTComposite GetRootComposite()
        {
            return root;
        }
    }
}
