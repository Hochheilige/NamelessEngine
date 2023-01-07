using Scripts.Components;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Scripts.BehaviorTree
{
    public class BTTreeExecutor
    {
        public BTTreeExecutor(BehaviorTreeComponent owner)
        {
            ownerBTComponent = owner;
        }

        private BehaviorTreeComponent ownerBTComponent;

        private BTTree tree;

        public void SetTreeToExecute(BTTree inTree)
        {
            tree = inTree;
            perNodeData.Clear();
            inProgressTask = null;
            inProgressTaskState = TaskStateEnum.InProgress;
        }

        class PerNodeData
        {
            public int prevIndex = (int)SpecialIndexValues.FirstTime;
            public TaskStateEnum prevResult = TaskStateEnum.InProgress;
        }

        Dictionary<BTComposite, PerNodeData> perNodeData = new Dictionary<BTComposite, PerNodeData>();
        BTTask inProgressTask = null;
        object inProgressTaskMemory = null;
        TaskStateEnum inProgressTaskState = TaskStateEnum.InProgress;

        public void StopInProgressTask(BTTask task, TaskStateEnum result)
        {
            if (task != inProgressTask)
            {
                throw new Exception("Trying to stop a task that is not currently in progress for this executor");
            }
            inProgressTaskState = result;
            inProgressTaskMemory = null;
            perNodeData[inProgressTask.GetParent()].prevResult = inProgressTaskState;
        }


        public void Execute(float deltaTime)
        {
            if (tree == null || ownerBTComponent == null)
            {
                return;
            }

            BTComposite comp = null;
            if (inProgressTask != null)
            {
                // tick inProgressTask and continue exectution from it if it finishes execution
                inProgressTask.TickTask(ownerBTComponent, inProgressTaskMemory, deltaTime);

                if (inProgressTaskState != TaskStateEnum.InProgress)
                { 
                    comp = inProgressTask.GetParent();
                    inProgressTask = null;
                }
                else
                {
                    return;
                }
            }

            if (comp == null) 
            {
                comp = tree.GetRootComposite();
                // starting from root - clear node data
                perNodeData.Clear();
            }

            if (comp == null)
            {
                return;
            }


            while (comp != null)
            {
                perNodeData.TryGetValue(comp, out PerNodeData nodeData);
                if (nodeData == null)
                {
                    nodeData = new PerNodeData();
                    perNodeData.Add(comp, nodeData);
                }

                int nextChildIndex = comp.GetNextChildIndex(nodeData.prevIndex, nodeData.prevResult);
                nodeData.prevIndex = nextChildIndex;
                if (nextChildIndex == (int)SpecialIndexValues.ReturnToParent)
                {
                    if (comp.GetParent() != null)
                        perNodeData[comp.GetParent()].prevResult = nodeData.prevResult;
                    comp = comp.GetParent();
                    continue;
                }
                BTCompositeChild nextChild = comp.GetChildAt(nextChildIndex);
                if (!nextChild.isTaskNode)
                {
                    comp = nextChild.childNode as BTComposite; 
                    continue;
                }
                else
                {
                    BTTask task = nextChild.childNode as BTTask;
                    object memory = task.CreateMemoryObject();
                    nodeData.prevResult = task.Execute(ownerBTComponent, memory);
                    if (nodeData.prevResult == TaskStateEnum.InProgress)
                    {
                        inProgressTask = task;
                        inProgressTaskMemory = memory;
                        inProgressTaskState = TaskStateEnum.InProgress;
                        return;
                    }
                }
            }
        }

    }
}
