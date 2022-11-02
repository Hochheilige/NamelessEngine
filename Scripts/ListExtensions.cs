using System;
using System.Collections.Generic;
using System.Linq;

namespace Scripts
{
    public static class ListExtensions
    {
        public static T GetComponent<T>(this List<Component> components) where T : Component
        {
            return components.First(x => x.GetType() == typeof(T)) as T;
        }
    }
}