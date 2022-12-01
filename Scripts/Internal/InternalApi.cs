using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using static Scripts.InputHandler;

namespace Scripts.Internal
{
    internal class InternalApi
    {
        private static void KeyPressed(int key)
        {
            Game.GetInstance().InputHandler.cpp_KeyPressed(key);
        }

        private static void KeyReleased(int key)
        {
            Game.GetInstance().InputHandler.cpp_KeyReleased(key);
        }
    }
}