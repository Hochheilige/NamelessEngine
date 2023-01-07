using Scripts.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public class BTSelector : BTComposite
    {
        public override int GetNextChildIndex(int prevIndex, TaskStateEnum prevResult)
        {
            if (prevIndex == (int)SpecialIndexValues.FirstTime && GetChildCount() > 0)
            {
                return 0;
            }
            else if (prevResult == TaskStateEnum.Failed && (prevIndex + 1) < GetChildCount())
            {
                return prevIndex + 1;
            }

            return (int)SpecialIndexValues.ReturnToParent;
        }
    }
}
