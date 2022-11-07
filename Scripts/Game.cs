using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

using ImGuiNET;

namespace Scripts
{
    public class Game
    {
        private static Game _instance;

        public readonly EngineSettings EngineSettings;


        internal Game()
        {
            if (_instance != null)
            {
                throw new InvalidOperationException("The game object has already been created.");
            }

            _instance = this;

            //TODO
            //var builder = new EngineSettings.Builder();
            //OnSpecifyEngineSettings(builder);
            //EngineSettings = builder.Build();
        }

        public static Game GetInstance()
        {
            if (_instance == null)
            {
                throw new InvalidOperationException("The game object has not been created yet.");
            }

            return _instance;
        }

        /**
         * Called upon start
         */
        internal virtual void OnLoad()
        {
            Console.WriteLine("Hello!");
        }

        /**
         * Called upon each tick
         */
        internal virtual void OnUpdate()
        {
           
        }

        internal virtual void OnGUI()
        {
            if (ImGui.GetCurrentContext() == null)
            {
                Console.WriteLine("No imGui Context");
                return;
            }
            else
            {
                ImGui.Begin("Testing testing c# window incoming");
                ImGui.Text("I came from C#!!!");
                //
                ImGui.End();
            }
        }

        internal void InitImGui(IntPtr contextPtr, IntPtr allocFunc, IntPtr freeFunc, IntPtr userData)
        {
            Console.WriteLine("Initing ImGui");
            ImGui.SetCurrentContext(contextPtr);

            ImGui.SetAllocatorFunctions(allocFunc, freeFunc, userData);
        }

        /**
         * Called upon exit
         */
        //TODO add calling from cpp
        internal virtual void OnExit()
        {

        }

        /**
         * Allows to modify engine settings upon game loading
         *///TODO add calling from cpp
        internal virtual void OnSpecifyEngineSettings(EngineSettings.Builder settingsBuilder)
        {

        }
    }
}
