using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using Scripts.Engine;

namespace Scripts.Extensions
{
    internal static class ComponentHelper
    {
        public static T GetComponent<T>(this List<Component> components, string name = null) where T : Component
        {
            var result = components.Where(x => x.GetType() == typeof(T)).ToList();

            if (result.Count > 0)
            {
                if (name != null)
                    return result.FirstOrDefault(x => x.Name == name) as T;

                return result.First() as T;
            }

            return null;
        }


        private static Dictionary<Type, ComponentType> Mapping;

        static ComponentHelper()
        {
            var fields = Enum.GetNames(typeof(ComponentType)).Select(n => typeof(ComponentType).GetField(n));

            var tempDic = fields.ToDictionary(
                f => (ComponentType)f.GetRawConstantValue(),
                f => f.GetCustomAttribute<ComponentTypeAttribute>()?.Type);

            var toRemove = tempDic.Where(x => x.Value == null).ToList();
            foreach (var pair in toRemove)
            {
                tempDic.Remove(pair.Key);
            }

            Mapping = tempDic.ToDictionary(x => x.Value, x => x.Key);
        }

        public static ComponentType GetEnumValueFromType<T>()
        {
            ComponentType type;
            if (!Mapping.TryGetValue(typeof(T), out type))
            {
                throw new ArgumentException("Component type was not introduced");
            }

            return type;
        }

    }
}