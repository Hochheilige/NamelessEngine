using Scripts.Components;
using Scripts.Tests;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    internal class BTTask_UpdateBlackboardWithPlayerPosition : BTTask
    {
        public BTTask_UpdateBlackboardWithPlayerPosition()
        {
            BlackboardKey = "";
        }
        public string BlackboardKey { get; set; }
        public override TaskStateEnum Execute(BehaviorTreeComponent btComponent, object nodeMemory)
        {
            if (BlackboardKey != "")
            {
                btComponent.blackboard[BlackboardKey] = Game.GetInstance().GetActorOfClass<PlayahActor>().GetTransform().Position;
                return TaskStateEnum.Succeded;

            }
            return TaskStateEnum.Failed;

        }
    }
}
