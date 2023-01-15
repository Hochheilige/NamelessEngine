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
        public BTTask_IsFarFromPlayer()
        {
            FarEnoughDistance = 5.0f;
        }

        public BTTask_IsFarFromPlayer(float farEnoughDistance)
        {
            FarEnoughDistance = farEnoughDistance;
        }

        public float FarEnoughDistance { get; set; }

        public override TaskStateEnum Execute(BehaviorTreeComponent btComponent, object nodeMemory)
        {
            Vector3 myPos = btComponent.GetOwner().GetTransform().Position;

            PlayahActor playah = Game.GetInstance().GetActorOfClass<PlayahActor>();

            float dist = (playah.GetTransform().Position - myPos).Length();

            return dist > FarEnoughDistance ? TaskStateEnum.Succeded : TaskStateEnum.Failed;
        }
    }
}
