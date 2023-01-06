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

        public enum KeyAction
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

        public enum MouseAction
        {
            Pressed,
            Released
        }
        
        internal void cpp_KeyPressed(int key)
        {
            var keyObj = (Keys)key;
            if (_pressedKeys.Add(keyObj))
            {
                // first press
                _game.OnKeyInput(keyObj, KeyAction.Pressed);
            }
            else
            {
                _game.OnKeyInput(keyObj, KeyAction.Repeated);
            }
        }

        internal void cpp_KeyReleased(int key)
        {
            var keyObj = (Keys)key;
            _pressedKeys.Remove((Keys)key);

            _game.OnKeyInput(keyObj, KeyAction.Released);
        }

        internal void cpp_MouseInput(int mouseButton, int mouseAction)
        {
            _game.OnMouseInput((MouseButton)mouseButton, (MouseAction)mouseAction);
        }
    }
}