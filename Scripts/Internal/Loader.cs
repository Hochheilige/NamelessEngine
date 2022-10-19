using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace Scripts.Internal
{
    internal class Loader
    {
        internal static object Boot()
        {
            object gameInstance = null;

            foreach (var assembly in AppDomain.CurrentDomain.GetAssemblies())
            {
                var entryPoints = GetTypesWithAttribute(assembly, typeof(EntryPoint));
                var count = entryPoints.Count();

                if (count == 0) continue;

                if (gameInstance != null || count != 1)
                {
                    throw new InvalidOperationException(
                        $"Entry point should be only one. Found: {string.Join(",", entryPoints)}");
                }

                var first = entryPoints.First();

                try
                {
                    gameInstance = Activator.CreateInstance(first);
                }
                catch (Exception e)
                {
                    throw new InvalidOperationException($"Can't create {first} object and use it as a game instance.", e);
                }
            }

            if (gameInstance == null)
            {
                throw new InvalidOperationException(
                    "No entry point found for engine to start.");
            }

            Console.WriteLine($"Found game instance to load: {gameInstance.GetType()}");

            return gameInstance;
        }

        private static IEnumerable<Type> GetTypesWithAttribute(Assembly assembly, Type attributeType)
        {
            return assembly.GetTypes().Where(t => t.IsDefined(attributeType));
        }
    }
}