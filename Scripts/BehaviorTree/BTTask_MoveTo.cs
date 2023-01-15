using Scripts.Components;
using Scripts.Extensions;
using SharpDX;
using System;
using System.Collections.Generic;
using System.Linq;
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
            return base.CreateMemoryObject();
        }


        public override TaskStateEnum Execute(BehaviorTreeComponent btComponent, object nodeMemory)
        {
            Actor owner = btComponent.GetOwner();

            var movementComponent = owner.Components.GetComponent<MovementComponent>();

            Vector3 targetPos = Position;
            if (UseBlackboardValue)
            {
                btComponent.blackboard.TryGetValue(BlackboardKey, out Object obj);
                if (obj is Vector3)
                {
                    targetPos = (Vector3)obj;
                }

            }

            if (movementComponent == null)
            {
                return TaskStateEnum.Failed;
            }

            if (!movementComponent.MoveTo(targetPos))
            {
                return TaskStateEnum.Failed;
            }

            return TaskStateEnum.InProgress;
        }

        public override void TickTask(BehaviorTreeComponent btComponent, object nodeMemory, float deltaTime)
        {
            Vector3 delta = btComponent.GetOwner().GetTransform().Position - Position;
            delta.Y = 0.0f;
            float distToTarget = delta.Length();
            if (distToTarget < AcceptanceRadius)
            {
                Actor owner = btComponent.GetOwner();
                var movementComponent = owner.Components.GetComponent<MovementComponent>();
                movementComponent.StopMoveTo();

                FinishLatentTask(btComponent, TaskStateEnum.Succeded);
            }
        }
    }
}
