using Scripts.Components;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using SharpDX;

namespace Scripts.BehaviorTree
{
    public class BTTask_RandomMove : BTTask
    {

        public BTTask_RandomMove() 
        {
            moveTime = 1.0f;
            radius = 2.0f;
        }

        public BTTask_RandomMove(float moveTime, float radius)
        {
            this.moveTime = moveTime;
            this.radius = radius;
        }

        public float moveTime { get; private set; }
        public float radius { get; private set; }
        public override object CreateMemoryObject()
        {
            return new BTTask_RandomMove_Memory();
        }

        public override TaskStateEnum Execute(BehaviorTreeComponent btComponent, object nodeMemory)
        {
            var mem = nodeMemory as BTTask_RandomMove_Memory;
            mem.remainingTime = moveTime;
            mem.startPosition = btComponent.GetOwner().GetTransform().Position;
            Random rnd = new Random();
            Vector3 dir = new Vector3(rnd.NextFloat(-1.0f, 1.0f), 0, rnd.NextFloat(-1.0f, 1.0f));
            dir.Normalize();
            mem.endPosition = mem.startPosition + dir * radius;
            return TaskStateEnum.InProgress;
        }

        public override void TickTask(BehaviorTreeComponent btComponent, object nodeMemory, float deltaTime)
        {
            var mem = nodeMemory as BTTask_RandomMove_Memory;

            mem.remainingTime -= deltaTime;

            var t = btComponent.GetOwner().GetTransform();
            float alpha = mem.remainingTime / moveTime;
            if (alpha < 0) alpha = 0.0f; 

            t.Position = mem.startPosition * alpha + mem.endPosition * (1-alpha);

            btComponent.GetOwner().SetTransform(t);

            if (mem.remainingTime < 0.0f)
            {
                FinishLatentTask(btComponent, TaskStateEnum.Succeded);
            }

        }
    }

    public class BTTask_RandomMove_Memory
    {
        public float remainingTime;
        public Vector3 startPosition;
        public Vector3 endPosition;
    }
}
