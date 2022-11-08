using System;
using System.Collections.Generic;
using System.Linq;

namespace Scripts
{
    public static class ListExtensions
    {
        public static T GetComponent<T>(this List<Component> components) where T : Component
        {
            var result = components.FirstOrDefault(x => x.GetType() == typeof(T)) as T;
            /*if (result is null)
            {
                throw new Exception("Component Was Not Found");
            }*/

            return result;
        }
    }
}