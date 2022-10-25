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

        Transform GetParentTransform();

        void Update(float deltaTime);
    }
}