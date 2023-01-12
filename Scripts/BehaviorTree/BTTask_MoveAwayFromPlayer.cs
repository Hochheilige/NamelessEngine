using Scripts.Components;
using Scripts.Engine;
using Scripts.Tests;
using SharpDX;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public class BTTask_MoveAwayFromPlayer : BTTask
    {

        public BTTask_MoveAwayFromPlayer() { }

        public BTTask_MoveAwayFromPlayer(float moveDistance, float moveTime)
        {
            this.moveDistance = moveDistance;
            this.moveTime = moveTime;
        }

        private float moveDistance = 5.0f;
        private float moveTime = 0.5f;

        public override object CreateMemoryObject()
        {
            return new BTTask_MoveAwayFromPlayer_Memory();
        }

        public override TaskStateEnum Execute(BehaviorTreeComponent btComponent, object nodeMemory)
        {
            var mem = nodeMemory as BTTask_MoveAwayFromPlayer_Memory;

            mem.timeReamaining = moveTime;

            mem.startPositon = btComponent.GetOwner().GetTransform().Position;

            Vector3 dir = mem.startPositon - Game.GetInstance().GetActorOfClass<PlayahActor>().GetTransform().Position;
            dir.Y = 0;
            dir.Normalize();

            mem.endPosition = mem.startPositon + dir * moveDistance;

            return TaskStateEnum.InProgress;
        }

        public override void TickTask(BehaviorTreeComponent btComponent, object nodeMemory, float deltaTime)
        {
            var mem = nodeMemory as BTTask_MoveAwayFromPlayer_Memory;

            mem.timeReamaining -= deltaTime;

            Actor owner = (Actor)btComponent.GetOwner();

            Transform t = owner.GetTransform();

            float alpha = mem.timeReamaining / moveTime;

            t.Position = mem.startPositon * alpha + mem.endPosition * (1 - alpha);

            owner.SetTransform(t);

            if (mem.timeReamaining < 0.0f)
            {
                FinishLatentTask(btComponent, TaskStateEnum.Succeded);
            }
        }

        class BTTask_MoveAwayFromPlayer_Memory
        {
            public float timeReamaining;

            public Vector3 startPositon;

            public Vector3 endPosition;
        }
    }
}
