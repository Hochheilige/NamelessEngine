using System;
using System.Linq;
using System.Reflection;

namespace Scripts.Engine.Meta
{
    public class MetaObj : Meta<PropertyInfoObj>
    {
        private static object _instance = null;
    
        public MetaObj(object instance) :  base(SetInstance(instance)) { }

        public dynamic GetPropertyValue(string propertyName)
        {
            return GetType().GetProperty(propertyName).GetValue(_instance);
        }
    
        public void SetPropertyValue(string propertyName, dynamic value)
        {
            GetType().GetProperty(propertyName).SetValue(_instance, value);
        }

        public override Type GetType()
        {
            return _instance.GetType();
        }
    
        protected override void InitializeProperties()
        {
            var propertyInfos = GetType().GetProperties().ToList();
            foreach (var property in propertyInfos)
            {
                Properties.Add(new PropertyInfoObj()
                {
                    Name = property.Name,
                    TypeName = property.PropertyType.ToString(),
                    Value = GetPropertyValue(property.Name)
                });
            }
        }

        private static string SetInstance(object instance)
        {
            _instance = instance;
            return instance.GetType().ToString();
        }
    }

    public class PropertyInfoObj : PropertyInfo
    {
        public dynamic Value { get; set; }
    }
}