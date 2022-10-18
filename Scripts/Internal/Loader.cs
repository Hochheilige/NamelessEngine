using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace Scripts.Internal
{
    public class Loader
    {
        public static void Main(string[] args)
        {
            Boot();
        }

        public static object Boot()
        {

            Console.Out.Flush();
            foreach (var assembly in AppDomain.CurrentDomain.GetAssemblies())
            {
                Console.WriteLine("2: " + assembly.FullName);
                Console.Out.Flush();
                var entryPoints = GetTypesWithAttribute(assembly, typeof(EntryPoint));
                // Console.WriteLine("3 " + string.Join(",", entryPoints));
                Console.Out.Flush();
                var count = entryPoints.Count();

                //if (count == 0) continue;

                //if (count != 1 || gameInstance != null)
                //{
                //    throw new InvalidOperationException(
                //        $"Entry point should be only one. Found: {string.Join(",", entryPoints)}");
                //}

                // var first = entryPoints.First();

                //try
                //{
                //    // gameInstance = Activator.CreateInstance(first);
                //}
                //catch (Exception e)
                //{
                //    // throw new InvalidOperationException($"Can't create {first} object and use it as a game instance.",
                //    //     e);
                //}
            }

            //if (gameInstance == null)
            //{
            //    //throw new InvalidOperationException(
            //    //    "No entry point found for engine to start.");
            //}

            return null;
        }

        private static IEnumerable<Type> GetTypesWithAttribute(Assembly assembly, Type attributeType)
        {
          // var length = GetLoadableTypes(assembly);
          return assembly.GetTypes().Where(t => t.IsDefined(attributeType));
           // return new List<Type>();
        }

        public static IEnumerable<Type> GetLoadableTypes(Assembly assembly)
        {
            if (assembly == null) throw new ArgumentNullException(nameof(assembly));
            try
            {
                return assembly.GetTypes();
            }
            catch (ReflectionTypeLoadException e)
            {
                Console.WriteLine("Failed");
                Console.WriteLine(string.Join(",", e.LoaderExceptions.OfType<Exception>()));
                Console.WriteLine(e.StackTrace);
                return e.Types.Where(t => t != null);
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                return null;
            }
        }
    }
}