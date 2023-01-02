using System;
using System.Collections.Generic;
using System.Linq;
using System.Security;
using System.Text;
using System.Threading.Tasks;
using ImGuiNET;
using imnodesNET;
using SharpDX;

namespace Scripts.BehaviorTree
{
    public class BehaviorTreeEditor
    {
        private List<EditorNode> editorNodes = new List<EditorNode>();
        private List<Link> editorLinks = new List<Link>();
        public void Init()
        {
            //temp stuff for testing
            //create root task

            //imnodesNET.IO io = *imnodes.GetIO();
            
            //io.LinkDetachWithModifierClick.Modifier = ImGui.GetIO().KeyCtrl;

            //imnodes.PushAttributeFlag(AttributeFlags.EnableLinkDetachWithDragClick);

            editorNodes.Add(new RootEditorNode(GetID(), GetID(),GetID()));
            //editorNodes.Add(new SequenceEditorNode(GetID(), GetID(), GetID()));
        }

        const int hardcoded_node_id = 1;
        public void Draw()
        {
            int start_link_id = -1;
            int end_link_id = -1;
            int link_id = -1;
            int node_id = -1;

            ImGui.Begin("Behavior Tree Editor");

            if (ImGui.BeginMenuBar())
            {
                if (ImGui.BeginMenu("Create"))
                {
                    ImGui.MenuItem("Sequence");
                    ImGui.MenuItem("Selector");
                    
                    ImGui.EndMenu();
                }
                if (ImGui.BeginMenu("stuff"))
                {
                    ImGui.MenuItem("Selector");
                }
                ImGui.EndMenuBar();
            }

            if (ImGui.Button("Sequence"))
            {
                CreateSequenceNode();
            }

            ImGui.SameLine();

            if (ImGui.Button("Selector"))
            {
                CreateSelectorNode();
            }

            ImGui.SameLine();

            if (ImGui.Button("Add task"))
            {
                CreateTaskNode();
            }



            imnodes.BeginNodeEditor();

            foreach (var node in editorNodes)
            {
                node.Draw();
            }

            foreach (var link in editorLinks)
            {
                link.Draw();
            }

            if (imnodes.IsEditorHovered() && ImGui.IsMouseClicked(ImGuiMouseButton.Right))
            {
                DrawCreateNode();
            }

            imnodes.EndNodeEditor();

            if (imnodes.IsLinkCreated(ref start_link_id, ref end_link_id))
            {
                editorLinks.Add(new Link(start_link_id, end_link_id, GetLinkID()));
            }

            if (imnodes.IsLinkDestroyed(ref link_id))
            {
                editorLinks.Remove(editorLinks.Find(l => l.link_id == link_id));
            }

            if (imnodes.IsLinkHovered(ref link_id) && ImGui.IsMouseClicked(ImGuiMouseButton.Middle))
            {
                editorLinks.Remove(editorLinks.Find(l => l.link_id == link_id));
            }

            ImGui.End();
        }

        private int id = 0;
        private int GetID()
        {
            ++id;
            return id;
        }

        private int link_id = 0; 
        private int GetLinkID()
        {
            return ++link_id;
        }

        private void DrawCreateNode()
        {
            //ImGui.PushID(1001);
            //ImGui.BeginGroup();

            ImGui.OpenPopup("NodeCreatorPopup");

            if (ImGui.BeginPopup("NodeCreatorPopup"))
            {
                if (ImGui.Selectable("Sequence"))
                {
                    CreateSequenceNode();
                }

                if (ImGui.Selectable("Selector"))
                {
                    CreateSequenceNode();
                }

                ImGui.EndPopup();
            }

            


            //ImGui.EndGroup();
            //ImGui.PopID();
        }

        private void CreateSequenceNode()
        {
            editorNodes.Add(new SequenceEditorNode(GetID(), GetID(), GetID()));
        }

        private void CreateSelectorNode()
        {
            editorNodes.Add(new SelectorEditorNode(GetID(), GetID(), GetID()));
        }

        private void CreateTaskNode()
        {
            editorNodes.Add(new TaskEditorNode(GetID(), GetID(), GetID()));
        }

        public abstract class EditorNode
        {
            public abstract void Draw();
            public int node_id;
            public Task task;
            public int input_id;
            public int output_id;
        }

        public class RootEditorNode : EditorNode
        {
            public RootEditorNode(int id, int input, int output)
            {
                node_id = id;
                task = new Sequence();
                input_id = input;
                output_id = output;
            }
            public override void Draw()
            {
                imnodes.BeginNode(node_id);

                imnodes.BeginNodeTitleBar();
                ImGui.Text("ROOT");
                imnodes.EndNodeTitleBar();

                imnodes.BeginOutputAttribute(node_id);
                ImGui.Text("children");
                imnodes.EndOutputAttribute();
                imnodes.EndNode();
            }
        }

        internal class SequenceEditorNode : EditorNode
        {
            public SequenceEditorNode(int id, int input, int output)
            {
                node_id = id;
                task = new Sequence();
                input_id = input;
                output_id = output;
            }
            public override void Draw()
            {
                imnodes.BeginNode(node_id);

                imnodes.BeginNodeTitleBar();
                ImGui.Text("SEQUENCE");
                imnodes.EndNodeTitleBar();

                imnodes.BeginInputAttribute(input_id);
                ImGui.Text("parent");
                imnodes.EndInputAttribute();
                imnodes.BeginOutputAttribute(output_id);
                ImGui.Text("children");
                imnodes.EndOutputAttribute();
                imnodes.EndNode();
            }
        }

        internal class SelectorEditorNode : EditorNode
        {
            public SelectorEditorNode(int id, int input, int output)
            {
                node_id = id;
                task = new Sequence();
                input_id = input;
                output_id = output;
            }
            public override void Draw()
            {
                imnodes.BeginNode(node_id);

                imnodes.BeginNodeTitleBar();
                ImGui.Text("SELECTOR");
                imnodes.EndNodeTitleBar();

                imnodes.BeginInputAttribute(input_id);
                ImGui.Text("parent");
                imnodes.EndInputAttribute();
                imnodes.BeginOutputAttribute(output_id);
                ImGui.Text("children");
                imnodes.EndOutputAttribute();
                imnodes.EndNode();
            }
        }

        internal class TaskEditorNode : EditorNode
        {
            public TaskEditorNode(int id, int input, int output)
            {
                node_id = id;
                task = new Sequence();
                input_id = input;
                output_id = output;
            }

            public override void Draw()
            {
                imnodes.BeginNode(node_id);

                imnodes.BeginNodeTitleBar();
                ImGui.Text("TASK");
                imnodes.EndNodeTitleBar();

                if (ImGui.Button("Choose Task"))
                {
                    if (ImGui.BeginPopup("TaskSelector"))
                    {

                        ImGui.Selectable("Task1");
                        ImGui.Selectable("Task2");
                        ImGui.Selectable("Task3");


                        ImGui.EndPopup();
                    }
                }

                imnodes.BeginInputAttribute(input_id);
                ImGui.Text("parent");
                imnodes.EndInputAttribute();
                imnodes.EndNode();
            }
        }

        internal class Link
        {
            public int start_id;
            public int end_id;
            public int link_id;

            public Link(int start, int end, int link_id)
            {
                start_id = start;
                end_id = end;
                this.link_id = link_id;
            }
            public void Draw()
            {
                imnodes.Link(link_id, start_id, end_id);
            }
        }

    }
}
