using SharpDX;

namespace Scripts.Engine
{
    public struct Transform
    {
        public Vector3 Position;
        public Quaternion Rotation;
        public Vector3 Scale;

        public override string ToString()
        {
            return $"{Position.X} {Position.Y} {Position.Z}";
        }
    }
}