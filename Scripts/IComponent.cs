using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts
{
    public interface IComponent
    {
        void OnRegister();

        void OnUnregister();

        float GetParentTransform();

        void Update();
    }
}
