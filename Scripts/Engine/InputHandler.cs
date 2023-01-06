using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using Scripts.Engine;
using System.Runtime.CompilerServices;
using Scripts.Internal;

namespace Scripts
{
    public class InputHandler
    {
        private readonly HashSet<Keys> _pressedKeys = new HashSet<Keys>();
        private readonly Game _game;

        public InputHandler(Game game)
        {
            _game = game;
        }

        //TODO uncomment
        //public delegate void MouseInputHandler();
        //public event MouseInputHandler OnMouseInput;

        //public delegate void MouseScrollHandler();
        //public event MouseScrollHandler OnMouseScroll;

        public bool IsKeyDown(Keys key)
        {
            return _pressedKeys.Contains(key);
        }

        public bool IsMouseButtonDown(MouseButton button)
        {
            return ExternalApi.IsMouseDown((int)button);
        }

        internal void cpp_KeyPressed(int key)
        {
            var keyObj = (Keys)key;
            if (_pressedKeys.Add(keyObj))
            {
                // first press
                Console.WriteLine($"Key Pressed: {keyObj} ({key})");
                _game.OnKeyInput(keyObj, ActionType.Pressed);
            }
            else
            {
                Console.WriteLine($"Key Repeated: {keyObj} ({key})");
                _game.OnKeyInput(keyObj, ActionType.Repeated);
            }
        }

        internal void cpp_KeyReleased(int key)
        {
            var keyObj = (Keys)key;
            Console.WriteLine($"Key Released: {keyObj} ({key})");
            _pressedKeys.Remove((Keys)key);

            _game.OnKeyInput(keyObj, ActionType.Released);
        }

        public enum ActionType
        {
            Pressed,
            Repeated,
            Released
        }

        public enum MouseButton
        {
            Left,
            Middle,
            Right
        }
    }
}