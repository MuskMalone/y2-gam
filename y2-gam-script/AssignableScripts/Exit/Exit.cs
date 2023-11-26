using Image;
using System;

namespace Object
{

    public class Exit : Entity
    {
        public Exit() : base()
        {

        }

        public Exit(uint entityHandle) : base(entityHandle)
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
                Vector2 exitEnd = new Vector2(Collider.X - (Scale.X / 2.0f), Collider.Y );
                if (PhysicsWrapper.Raycast(Collider, exitEnd, entityID, out RaycastHit exitHit) && exitHit.tag == "Player")
                {
                    //Console.WriteLine("Exit");
                    LoadScene("MainMenu");
                }

            }
        }

        void OnExit()
        {

        }

    }
}
