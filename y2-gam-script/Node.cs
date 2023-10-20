using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Image;

namespace Object
{
    public class Node : Entity
    {
        protected List<Node> neighbourList;
        public Node() : base() { }
        public Node(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        void OnCreate()
        {

        }

        void OnUpdate(float dt)
        {

        }
    }
}
