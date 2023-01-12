using Scripts.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public abstract class BTComposite : BTNode
    {
        protected List<BTCompositeChild> children = new List<BTCompositeChild>();

        public abstract int GetNextChildIndex(int prevIndex, TaskStateEnum prevResult);

        public BTCompositeChild GetChildAt(int index)
        {
            return children[index];
        }

        public int GetChildCount() { return children.Count; }

        public void AddChild(BTNode node)
        {
            if (node == null) throw new ArgumentNullException("Trying to add a null node as a child");

            node.SetParent(this);
            BTCompositeChild child = new BTCompositeChild();
            child.childNode = node;
            child.isTaskNode = node is BTTask;
            children.Add(child);
        }

        // for internal use only
        public void SortChildren(Dictionary<BTNode, int> ordinals)
        {
            children = children.OrderBy(child => ordinals[child.childNode]).ToList();
        }
    }

    public enum SpecialIndexValues
    {
        FirstTime = -1,
        ReturnToParent = -2
    }

    public class BTCompositeChild
    {
        // either composite or a task
        public BTNode childNode = null;

        public bool isTaskNode = false;

        // todo: list of decorators here?
    }
}
