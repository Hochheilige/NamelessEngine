using System.Runtime.CompilerServices;

namespace Scripts.Engine
{
    public static class LevelLoader
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void LoadLevelInternal(string levelName);
        
        public static void LoadLevel(string levelName)
        {
            LoadLevelInternal(@"Assets\Levels\" + levelName);
            //LoadLevelInternal(levelName);
        }
    }
}