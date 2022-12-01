using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace Scripts
{
    public class InputHandler
    {
        private readonly HashSet<Keys> _pressedKeys = new HashSet<Keys>();

        public delegate void KeyInputHandler(Keys key, ActionType action);
        public event KeyInputHandler OnKeyInput;

        //TODO uncomment
        //public delegate void MouseInputHandler();
        //public event MouseInputHandler OnMouseInput;

        //public delegate void MouseScrollHandler();
        //public event MouseScrollHandler OnMouseScroll;

        public bool IsKeyDown(Keys key)
        {
            return _pressedKeys.Contains(key);
        }

        internal void cpp_KeyPressed(int key)
        {
            var keyObj = (Keys)key;
            if (_pressedKeys.Add(keyObj))
            {
                // first press
                Console.WriteLine($"Key Pressed: {keyObj} ({key})");
                OnKeyInput?.Invoke(keyObj, ActionType.Pressed);
            }
            else
            {
                Console.WriteLine($"Key Repeated: {keyObj} ({key})");
                OnKeyInput?.Invoke(keyObj, ActionType.Repeated);
            }
        }

        internal void cpp_KeyReleased(int key)
        {
            var keyObj = (Keys)key;
            Console.WriteLine($"Key Released: {keyObj} ({key})");
            _pressedKeys.Remove((Keys)key);

            OnKeyInput?.Invoke(keyObj, ActionType.Released);
        }

        public enum ActionType
        {
            Pressed,
            Repeated,
            Released
        }
    }
}