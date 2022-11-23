using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts
{
    public class InputHandler
    {
        public HashSet<Keys> keys;

        public delegate void OnKeyInput(Keys key);
        public static event OnKeyInput onKeyInput;

        public delegate void OnMouseInput();
        public static event OnMouseInput onMouseInput;

        public delegate void OnMouseScroll();
        public static event OnMouseScroll onMouseScroll;

        private void AddPressedKey(int key)
        {
            //onKeyInput?.Invoke((Keys)key);
            keys.Add((Keys)key);
        }
        
        private void RemovePressedKey(int key)
        {
            //onKeyInput?.Invoke((Keys)key);
            keys.Remove((Keys)key);
        }

        public bool IsKeyPressed(Keys key)
        {
            return keys.Contains(key);
        }
        
        public void FireKeyboardEvent(int key)
        {
            //Console.WriteLine("event with " + key);
            onKeyInput?.Invoke((Keys)key);
        }
    }
}