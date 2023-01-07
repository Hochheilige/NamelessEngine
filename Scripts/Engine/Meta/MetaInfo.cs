using System;
using System.Linq;

namespace Scripts.Engine.Meta
{
    public class MetaInfo : Meta<PropertyInfo>
    {
        public MetaInfo(){}
        public MetaInfo(string fullName) : base(fullName) { }

        public PropertyInfo GetField(string name)
        {
            return Properties.FirstOrDefault(x => x.Name == name);
        }

        protected override void InitializeProperties()
        {
            Console.WriteLine("meta info");
            var propertyInfos = GetType().GetProperties().ToList();
            foreach (var property in propertyInfos)
            {
                Properties.Add(new PropertyInfo()
                {
                    Name = property.Name,
                    TypeName = property.PropertyType.ToString()
                });
            }
        }
    }
}