using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts
{
    class InputHandler
    {
        public delegate void OnKeyInput(int button);
        public static event OnKeyInput onKeyInput;

        public delegate void OnMouseInput();
        public static event OnMouseInput onMouseInput;

        public delegate void OnMouseScroll();
        public static event OnMouseScroll onMouseScroll;
    }
}