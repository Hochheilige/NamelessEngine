using System;
using Scripts.Components;
using Scripts.Engine;
using Scripts.Extensions;
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
            mv_cmp.SetGenerateOverlapEvents(true);
            camComp = AddComponent<CameraComponent>("CameraComponent");
        }

        private bool isJumpPressed = false;
        private float characterSpeed = 10.0f;
        private Vector3 jumpDirection = new Vector3(0, 10, 0);
        private Vector3 movementDelta = new Vector3(0, 0, 0);
        private float airControlModifier = 0.15f;
        private float jumpSpeadModifier = 3.0f;
        private Vector3 playerGravity = new Vector3(0, -9.81f, 0);

        float Pitch = 0.0f;
        float Yaw = 0.0f;

        float MaxPitch = 80.0f;
        float RotSpeedYaw = 40000.0f;
        float RotSpeedPitch = 12000.0f;
        float OrbitRadius = 10.0f;
        private Vector3 bulletOffset = Vector3.ForwardLH * 2.0f;
        private float bulletSpeed = 50f;
        private Vector3 cameraOffset = new Vector3(1.5f, 1.0f, 0.0f);



        private float fireDelay = 0.5f;
        private float currentFireDelay = -0.1f;

        private Transform cameraTransform;

        public override void Update(float deltaTime)
        {
            var inputHandler = Game.GetInstance().InputHandler;
            if (inputHandler.IsKeyDown(Keys.Space) && mv_cmp.CanJump())
            {
                if (!isJumpPressed) { mv_cmp.Jump(jumpDirection + movementDelta * characterSpeed * jumpSpeadModifier); }
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



            float deltaX = camComp.GetMouseDeltaX();
            float deltaY = camComp.GetMouseDeltaY();
            // TODO: use mouse data when it has been added to C#
            //inputHandler.GetMouse()->GetDeltas(deltaX, deltaY);
            //if (inputHandler.IsKeyDown(Keys.Left))
            //{
            //    deltaX -= 1.0f;
            //}
            //if (inputHandler.IsKeyDown(Keys.Right))
            //{
            //    deltaX += 1.0f;
            //}

            //if (inputHandler.IsKeyDown(Keys.Up))
            //{
            //    deltaY -= 1.0f;
            //}
            //if (inputHandler.IsKeyDown(Keys.Down))
            //{
            //    deltaY += 1.0f;
            //}

            Pitch += deltaTime * deltaY * RotSpeedPitch;
            Pitch = Pitch < -MaxPitch ? -MaxPitch : Pitch > MaxPitch ? MaxPitch : Pitch;
            Yaw += deltaTime * deltaX * RotSpeedYaw;

            //OrbitRadius += orbitRadiusDelta;

            // todo: ignore only GCToOrbit's yaw and pitch rotation and scale

            Matrix moveMat = Matrix.RotationQuaternion(cameraTransform.Rotation);
            Vector3 right = moveMat.Right;
            Vector3 forward = Vector3.Cross(right, new Vector3(0, 1, 0));

            Matrix mat = Matrix.Translation(new Vector3(0.0f, 0.0f, OrbitRadius)) *
                Matrix.RotationYawPitchRoll(Yaw * (float)Math.PI / 180.0f, Pitch * (float)Math.PI / 180.0f, 0.0f) *
                Matrix.Translation(GetTransform().Position);

            mat.Decompose(out Vector3 scale, out Quaternion rot, out Vector3 pos);
            cameraTransform.Position = pos;
            cameraTransform.Rotation = rot;
            cameraTransform.Scale = scale;
            Vector3 camMod = cameraOffset;
            camMod -= cameraOffset.Z * forward;
            camMod += cameraOffset.X * right;
            camComp.SetCameraTransform(pos, rot);

            //Walking
            movementDelta = new Vector3(0, 0, 0);
            

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

            if (!mv_cmp.CanJump()) movementDelta *= airControlModifier;

            mv_cmp.SetWalkDirection(movementDelta);

        }

        private void fire()
        {
            Bullet bullet = Instantiator.InstantiateActor<Bullet>();
            bullet.PublicInit();

            Matrix moveMat = Matrix.RotationQuaternion(cameraTransform.Rotation);
            Transform t = GetTransform();
            Vector3 right = moveMat.Right;
            Vector3 forward = Vector3.Cross(right, new Vector3(0, 1, 0));
            t.Position -= bulletOffset.Z * forward;
            t.Position += bulletOffset.X * right;
            bullet.SetTransform(t);

           

            bullet.SetInittialSpeed(moveMat.Forward * bulletSpeed);

            bullet.fire();

            currentFireDelay = fireDelay;
        }

        public override void OnBeginPlay()
        {
            
        }

        public override void SetTransform(Transform transform)
        {

        }

        protected override void Hit(Actor otherActor)
        {
            //otherActor.Components.GetComponent<StaticMeshRenderer>().SetMeshPath("../Assets/Meshes/test.fbx/Torus");
            Console.WriteLine("PlayahActor Hit event with {0}", otherActor.ToString());
        }   
        protected override void BeginOverlap(Actor otherActor)
        {
            //otherActor.Components.GetComponent<StaticMeshRenderer>().SetMeshPath("../Assets/Meshes/test.fbx/Torus");
            Console.WriteLine("PlayahActor BeginOverlap event with {0}", otherActor.ToString());
        }
        protected override void EndOverlap (Actor otherActor)
        {
            //otherActor.Components.GetComponent<StaticMeshRenderer>().SetMeshPath("../Assets/Meshes/test.fbx/Torus");
            Console.WriteLine("PlayahActor EndOverlap event with {0}", otherActor.ToString());
        }

    }
}
