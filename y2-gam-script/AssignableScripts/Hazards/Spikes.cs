using Image;
using System;
using System.Runtime.Remoting.Metadata.W3cXsd2001;

namespace Object
{
    public class Spike : Entity
    {

        public Spike() : base()
        {

        }

        public Spike(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

    }

   

}
