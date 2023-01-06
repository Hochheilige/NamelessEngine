﻿using Scripts.Components;
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

        private float moveTime = 1.0f;
        private float radius = 2.0f;
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
            Vector3 dir = new Vector3(rnd.Next(), 0, rnd.Next());
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
