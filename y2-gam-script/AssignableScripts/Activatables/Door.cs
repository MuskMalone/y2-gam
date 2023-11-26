using Image;
using System;

namespace Object
{

    public class Door : Entity
    {
        public Door() : base()
        {

        }

        public Door(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        void OnCreate()
        {

        }



        void OnUpdate(float dt)
        {
            if (!IsEditorMode())
            {
                //if(GameplayWrapper.IsPressed == false)
                //{
                //    GameplayWrapper.DestroyEntity(entityID);
                
                //}
            }
        }

        void OnExit()
        {

        }

    }
}
