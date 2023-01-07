using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace Scripts.Engine.Meta
{
    public abstract class Meta<TField> where TField : PropertyInfo
    {
        public Meta(string fullName)
        {
            var dividerIndex = fullName.LastIndexOf('.');
            Namespace = fullName.Substring(0, fullName.LastIndexOf('.'));
            Name = fullName.Substring(dividerIndex + 1, fullName.Length - dividerIndex - 1);
        
            InitializeProperties();
        }
    
        public Meta(){}

        public string Namespace { get; set; }
    
        public string Name { get; set; }
    
        public virtual Type GetType()
        {
            Assembly asm = typeof(PropertyInfo).Assembly;
            return asm.GetType(Namespace + "." + Name);
        }

        public List<TField> Properties { get; set; } = new List<TField>();

        protected abstract void InitializeProperties();
    }
    
    public class PropertyInfo
    {
        public string Name { get; set; }

        public string TypeName { get; set; }

    }
}