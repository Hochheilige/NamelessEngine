using Scripts.Components;
using Scripts.Tests;
using SharpDX;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public class BTTask_IsFarFromPlayer : BTTask
    {
        public BTTask_IsFarFromPlayer() { }

        public BTTask_IsFarFromPlayer(float farEnoughDistance)
        {
            this.farEnoughDistance = farEnoughDistance;
        }

        private float farEnoughDistance = 5.0f;

        public override TaskStateEnum Execute(BehaviorTreeComponent btComponent, object nodeMemory)
        {
            Vector3 myPos = btComponent.GetOwner().GetTransform().Position;

            PlayahActor playah = Game.GetInstance().GetActorOfClass<PlayahActor>();

            float dist = (playah.GetTransform().Position - myPos).Length();

            return dist > farEnoughDistance ? TaskStateEnum.Succeded : TaskStateEnum.Failed;
        }
    }
}
