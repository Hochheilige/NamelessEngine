using System;
using Scripts.Components;
using SharpDX;

namespace Scripts.Tests
{
    internal class PlayahActor : Actor
    {
        private MovementComponent mv_cmp;
        public PlayahActor() {
            //add components here
            mv_cmp = (MovementComponent)AddComponent((int)ComponentsEnum.MovementComponentType);
        }

        private bool isJumpPressed = false;
        private float characterSpeed = 4.0f;

        public override void Update(float deltaTime)
        {
            var inputHandler = Game.GetInstance().InputHandler;
            if (inputHandler.IsKeyDown(Keys.Space))
            {
                if (!isJumpPressed) { mv_cmp.Jump(); }
                isJumpPressed = true;
            } 
            else
            {
                isJumpPressed = false;
            }

            //Walking

            Vector3 movementDelta = new Vector3(0, 0, 0);

            if (inputHandler.IsKeyDown(Keys.D))
            {
                movementDelta.X += deltaTime * characterSpeed;
            }
            if (inputHandler.IsKeyDown(Keys.A))
            {
                movementDelta.X -= deltaTime * characterSpeed;
            }
            if (inputHandler.IsKeyDown(Keys.W))
            {
                movementDelta.Z += deltaTime * characterSpeed;
            }
            if (inputHandler.IsKeyDown(Keys.S))
            {
                movementDelta.Z -= deltaTime * characterSpeed;
            }

            mv_cmp.SetWalkDirection(movementDelta);
        }

        public override void OnBeginPlay()
        {
            
        }

    }
}
