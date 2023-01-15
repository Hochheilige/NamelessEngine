using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public class BehaviorTreeManager
    {
        private BehaviorTreeManager() 
        {
            
        }

        public static BehaviorTreeManager Instance { get { if (instance == null) instance = new BehaviorTreeManager(); return instance; } }

        private static BehaviorTreeManager instance = null;

        public BTTree GetTree(string path)
        {
            openedTrees.TryGetValue(path, out var tree);
            if (tree == null)
            {
                tree = new BTTree(path);
                openedTrees.Add(path, tree);
            }
            return tree;
        }

        public void ReloadOpenedTrees()
        {
            foreach (var pair in openedTrees)
            {
                var tree = pair.Value;
                tree.InitWithPath(pair.Key);
            }
        }

        Dictionary<string, BTTree> openedTrees = new Dictionary<string, BTTree>();
    }
}
