using System;
using Scripts.Components;
using Scripts.Engine;
using SharpDX;

namespace Scripts.Tests
{
    internal class PlayahActor : Actor
    {
        private MovementComponent mv_cmp;
        private CameraComponent camComp;
        public PlayahActor() {
           
        }

        protected internal override void RegisterComponents()
        {
            //add components here
        }

        protected internal override void Init()
        {
            mv_cmp = AddComponent<MovementComponent>("MovementComponent");
            camComp = AddComponent<CameraComponent>("CameraComponent");
        }

        private bool isJumpPressed = false;
        private float characterSpeed = 4.0f;
        private Vector3 jumpDirection = new Vector3(0, 10, 0);

        float Pitch = 0.0f;
        float Yaw = 0.0f;

        float MaxPitch = 80.0f;
        float RotSpeedYaw = 200.0f;
        float RotSpeedPitch = 200.0f;
        float OrbitRadius = 10.0f;

        private float fireDelay = 0.5f;
        private float currentFireDelay = -0.1f;

        private Transform cameraTransform;

        public override void Update(float deltaTime)
        {
            var inputHandler = Game.GetInstance().InputHandler;
            if (inputHandler.IsKeyDown(Keys.Space))
            {
                if (!isJumpPressed) { mv_cmp.Jump(jumpDirection); }
                isJumpPressed = true;
            }
            else
            {
                isJumpPressed = false;
            }

            // shooting 

            if (currentFireDelay >= 0.0f)
            {
                currentFireDelay -= deltaTime;
            }

            if (currentFireDelay < 0.0f && inputHandler.IsKeyDown(Keys.F))
            {
                fire();
            }

            float deltaX = 0.0f;
            float deltaY = 0.0f;
            // TODO: use mouse data when it has been added to C#
            //inputHandler.GetMouse()->GetDeltas(deltaX, deltaY);
            if (inputHandler.IsKeyDown(Keys.Left))
            {
                deltaX -= 1.0f;
            }
            if (inputHandler.IsKeyDown(Keys.Right))
            {
                deltaX += 1.0f;
            }

            if (inputHandler.IsKeyDown(Keys.Up))
            {
                deltaY -= 1.0f;
            }
            if (inputHandler.IsKeyDown(Keys.Down))
            {
                deltaY += 1.0f;
            }

            Pitch += deltaTime * deltaY * RotSpeedPitch;
            Pitch = Pitch < -MaxPitch ? -MaxPitch : Pitch > MaxPitch ? MaxPitch : Pitch;
            Yaw += deltaTime * deltaX * RotSpeedYaw;

            //OrbitRadius += orbitRadiusDelta;

            // todo: ignore only GCToOrbit's yaw and pitch rotation and scale


            Matrix mat = Matrix.Translation(new Vector3(0.0f, 0.0f, OrbitRadius)) *
                Matrix.RotationYawPitchRoll(Yaw * (float)Math.PI / 180.0f, Pitch * (float)Math.PI / 180.0f, 0.0f) *
                Matrix.Translation(GetTransform().Position);

            mat.Decompose(out Vector3 scale, out Quaternion rot, out Vector3 pos);
            cameraTransform.Position = pos;
            cameraTransform.Rotation = rot;
            cameraTransform.Scale = scale;
            camComp.SetCameraTransform(pos, rot);

            //Walking

            Vector3 movementDelta = new Vector3(0, 0, 0);
            Matrix moveMat = Matrix.RotationQuaternion(cameraTransform.Rotation);
            Vector3 right = moveMat.Right;
            Vector3 forward = Vector3.Cross(right, new Vector3(0, 1, 0));

            float forwardInput = 0.0f;
            float rightInput = 0.0f;

            if (inputHandler.IsKeyDown(Keys.W))
            {
                forwardInput += deltaTime * characterSpeed;
            }
            if (inputHandler.IsKeyDown(Keys.S))
            {
                forwardInput -= deltaTime * characterSpeed;
            }
            if (inputHandler.IsKeyDown(Keys.D))
            {
                rightInput += deltaTime * characterSpeed;
            }
            if (inputHandler.IsKeyDown(Keys.A))
            {
                rightInput -= deltaTime * characterSpeed;
            }


            movementDelta += right * rightInput;
            movementDelta -= forward * forwardInput;

            mv_cmp.SetWalkDirection(movementDelta);

        }

        private void fire()
        {
            Bullet bullet = Instantiator.InstantiateActor<Bullet>();
            bullet.PublicInit();
            bullet.SetTransform(GetTransform());
            
            bullet.fire();

            currentFireDelay = fireDelay;
        }

        public override void OnBeginPlay()
        {
            
        }

        public override void SetTransform(Transform transform)
        {

        }

        protected override void Overlap()
        {
            Console.WriteLine("PlayahActor Overlap event");
        }

    }
}
