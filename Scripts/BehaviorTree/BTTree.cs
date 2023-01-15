using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public class BTTree
    {
        public BTTree()
        {

        }

        public BTTree(string path)
        {
            InitWithPath(path);
        }

        public void InitWithPath(string path)
        {
            if (!File.Exists(path))
                return;

            root = new BTSequence();

            JObject data = JObject.Parse(File.ReadAllText(path));

            JObject treeData = data["TreeData"] as JObject;

            JArray nodes = treeData["Nodes"] as JArray;
            JArray links = treeData["Links"] as JArray;

            List<KeyValuePair<JObject, BTNode>> nodeDatas = new List<KeyValuePair<JObject, BTNode>>();

            Dictionary<BTNode, int> nodeTOrdinalMap = new Dictionary<BTNode, int>();

            // Create all nodes
            foreach (JObject jnode in nodes)
            {
                string nodeKind = (string)jnode["Kind"];
                int ordinal = (int)jnode["Ordinal"];
                // skip unconnected nodes
                if (ordinal == -1 && nodeKind != "Root")
                    continue;
                BTNode newNode = null;
                if (nodeKind == "Root")
                {
                    newNode = this.root;
                }
                else if (nodeKind == "Sequence")
                {
                    newNode = new BTSequence();
                }
                else if (nodeKind == "Selector")
                {
                    newNode = new BTSelector();
                }
                else if (nodeKind == "Task")
                {
                    string objectToInstantiate = (string)jnode["taskData"]["Namespace"] + "." + (string)jnode["taskData"]["Name"] + ", " + Assembly.GetAssembly(typeof(BTTask));
                    var objectType = Type.GetType(objectToInstantiate);
                    newNode = Activator.CreateInstance(objectType) as BTNode;

                    foreach (JObject prop in jnode["taskData"]["Properties"])
                    {
                        PropertyInfo pi = newNode.GetType().GetProperty((string)prop["Name"]);
                        if (pi != null)
                        {
                            // todo: fixe this hack
                            if (pi.PropertyType == typeof(SharpDX.Vector3))
                            {
                                string test = JsonConvert.SerializeObject(new SharpDX.Vector3(0.0f, 0.0f, 0.0f));
                                SharpDX.Vector3 val = new SharpDX.Vector3((float)prop["Value"]["X"], (float)prop["Value"]["Y"], (float)prop["Value"]["Z"]);
                                pi.SetValue(newNode, val);
                            }
                            else
                            {
                                var getJsonValue = typeof(JToken).GetMethod("Value").MakeGenericMethod(pi.PropertyType);
                                pi.SetValue(newNode, getJsonValue.Invoke(prop, new object[] { "Value" }));
                            }
                        }
                    }
                }

                KeyValuePair<JObject, BTNode> nodeData = new KeyValuePair<JObject, BTNode>(jnode, newNode);
                nodeDatas.Add(nodeData);

                nodeTOrdinalMap.Add(newNode, ordinal);
            }

            // Connect the nodes using links data
            foreach (JObject link in links)
            {
                ulong startPinId = (ulong)link["StartPinID"];
                ulong endPinId = (ulong)link["EndPinID"];

                // find node with start pin id
                KeyValuePair<JObject, BTNode> startNode = nodeDatas.Find(nd => {
                    JArray outputs = nd.Key["Outputs"] as JArray;
                    if (outputs == null)
                        return false;
                    foreach (JObject outPin in outputs)
                    {
                        if ((ulong)outPin["ID"] == startPinId)
                            return true;
                    }
                    return false;
                });
                // find node with end pin id
                KeyValuePair<JObject, BTNode> endNode = nodeDatas.Find(nd => {
                    JArray inputs = nd.Key["Inputs"] as JArray;
                    if (inputs == null)
                        return false;
                    foreach (JObject inPin in inputs)
                    {
                        if ((ulong)inPin["ID"] == endPinId)
                            return true;
                    }
                    return false;
                });
                // if a start node is not composite then smth is wrong
                (startNode.Value as BTComposite).AddChild(endNode.Value);
            }

            // sort each node's children by ordinals
            foreach (var nd in nodeDatas)
            {
                BTComposite comp = nd.Value as BTComposite;
                if (comp != null)
                {
                    comp.SortChildren(nodeTOrdinalMap);
                }
            }
        }

        // hack: use a sequence with one child as root
        private BTSequence root = new BTSequence();

        public BTNode GetChildOfRoot()
        {
            return root.GetChildCount() > 0 ? root.GetChildAt(0).childNode : null;
        }

        // for internal use only!!!!
        public BTComposite GetRootNode()
        {
            return root;
        }

        public void SetRoot(BTComposite root)
        {
            this.root.AddChild(root);
        }
    }
}
