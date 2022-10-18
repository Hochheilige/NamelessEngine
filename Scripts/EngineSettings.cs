using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts
{
    public class EngineSettings
    {

        //TODO here you can add any props you want to be handled by user upon start

        public class Builder
        {
            public EngineSettings Build()
            {
                return new EngineSettings();
            }
        }
    }
}
