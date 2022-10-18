using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.Tests
{
    [EntryPoint]
    public class TestGame : Game
    {
        internal override void OnLoad()
        {
            Console.WriteLine("Me booted, kek.");
        }
    }
}
