using Scripts.Components;
using Scripts.Extensions;
using SharpDX;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public class BTTask_MoveTo : BTTask
    {
        public BTTask_MoveTo() 
        {
            Position = new Vector3(0.0f, 0.0f, 0.0f);
            UseBlackboardValue = false;
            BlackboardKey = "";
            AcceptanceRadius = 1.0f;
        }

        public Vector3 Position { get; set; }
        public bool UseBlackboardValue { get; set; }
        public string BlackboardKey { get; set; }
        public float AcceptanceRadius { get; set; }

        public override object CreateMemoryObject()
        {
            return new BTTask_MoveTo_Memory();
        }


        public override TaskStateEnum Execute(BehaviorTreeComponent btComponent, object nodeMemory)
        {
            Actor owner = btComponent.GetOwner();

            var movementComponent = owner.Components.GetComponent<MovementComponent>();

            var mem = nodeMemory as BTTask_MoveTo_Memory;

            mem.targetPos = Position;
            if (UseBlackboardValue)
            {
                btComponent.blackboard.TryGetValue(BlackboardKey, out Object obj);
                if (obj is Vector3)
                {
                    mem.targetPos = (Vector3)obj;
                }

            }

            if ((mem.targetPos - owner.GetTransform().Position).Length() < AcceptanceRadius)
            {
                return TaskStateEnum.Succeded;
            }

            if (movementComponent == null)
            {
                return TaskStateEnum.Failed;
            }

            if (!movementComponent.MoveTo(mem.targetPos))
            {
                return TaskStateEnum.Failed;
            }

            return TaskStateEnum.InProgress;
        }

        public override void TickTask(BehaviorTreeComponent btComponent, object nodeMemory, float deltaTime)
        {
            var mem = nodeMemory as BTTask_MoveTo_Memory;

            Vector3 delta = btComponent.GetOwner().GetTransform().Position - mem.targetPos;
            float distToTarget = delta.Length();

            Actor owner = btComponent.GetOwner();
            var movementComponent = owner.Components.GetComponent<MovementComponent>();

            if (distToTarget < AcceptanceRadius)
            {
                
                movementComponent.StopMoveTo();

                FinishLatentTask(btComponent, TaskStateEnum.Succeded);
            }
            else if (!movementComponent.IsFollowingNavPath())
            {
                FinishLatentTask(btComponent, TaskStateEnum.Failed);
            }
        }
    }
    
    class BTTask_MoveTo_Memory
    {
        public Vector3 targetPos;
    }
}
