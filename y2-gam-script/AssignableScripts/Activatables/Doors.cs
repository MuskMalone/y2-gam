//using Image;
//using System;

//namespace Object
//{

//    public class Doors : Entity
//    {
//        public Doors() : base()
//        {

//        }

//        public Doors(uint entityHandle) : base(entityHandle)
//        {
//            entityID = entityHandle;
//        }

//        void OnCreate()
//        {
//            Vector2 playerFeet = new Vector2(Translation.X, Translation.Y - (Scale.Y / 2.0f) - 2.0f);
//            Vector2 spikesTip = new Vector2(Translation.X, Translation.Y - (Scale.Y / 2.0f) - 2.0f);

//            if (PhysicsWrapper.Raycast(playerFeet, spikesTip, entityID, out RaycastHit spikeHit))
//            {

//                if (spikeHit.tag == "Spikes")
//                {

//                    }
//                }
//            }

//        }

//        void OnUpdate(float dt)
//        {
//        }

//        void OnExit()
//        {

//        }
//    }
//}
