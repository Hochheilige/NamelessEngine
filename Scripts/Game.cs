﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

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

        }

        /**
         * Called upon each tick
         */
        internal virtual void OnUpdate()
        {

        }

        /**
         * Called upon exit
         */
        internal virtual void OnExit()
        {

        }

        /**
         * Allows to modify engine settings upon game loading
         */
        internal virtual void OnSpecifyEngineSettings(EngineSettings.Builder settingsBuilder)
        {

        }
    }
}