using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using ImGuiNET;
using imnodesNET;
using Scripts.BehaviorTree;
using Scripts.Engine;

namespace Scripts
{
    public class Game
    {
        private List<Actor> actors = new List<Actor>();
        
        private static Game _instance;

        public readonly InputHandler InputHandler = new InputHandler();

        public readonly EngineSettings EngineSettings;

        private BehaviorTreeEditor btEditor = new BehaviorTreeEditor();


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

        public IEnumerable<T> GetActorsOfClass<T>()
        {
            var objs = actors.Where(x => x.GetType() == typeof(T)).ToList();
            if (objs.Any())
            {
                return objs.Cast<T>();
            }

            return new List<T>();
        }

        public void AddActor(Actor actor) { actors.Add(actor); }

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
        
        internal virtual string GetActorInheritors()
        {
            var listOfBs = (
                from domainAssembly in AppDomain.CurrentDomain.GetAssemblies()
                from type in domainAssembly.GetTypes()
                where typeof(Actor).IsAssignableFrom(type)
                select type.ToString()).ToArray();
            
            var result = string.Join("\n", listOfBs);
            //Console.WriteLine(result);
            return result;
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
                btEditor.Draw();
                //ImGui.Begin("Testing testing c# window incoming");
                //ImGui.Text("I came from C#!!!");
                ////
                //ImGui.End();
            }
        }

        internal void InitImGui(IntPtr contextPtr, IntPtr allocFunc, IntPtr freeFunc, IntPtr userData)
        {
            ImGui.SetCurrentContext(contextPtr);

            ImGui.SetAllocatorFunctions(allocFunc, freeFunc, userData);

            //init imnodes
            imnodes.SetImGuiContext(contextPtr);
            imnodes.Initialize();

            //for fun
            btEditor.Init();
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
