using System;
using System.Collections.Generic;
using System.Linq;

namespace Scripts
{
    public static class ListExtensions
    {
        public static Component GetComponent<T>(this List<Component> components)
        {
            return components.First(x => x.GetType() == typeof(T));
        }
    }
}