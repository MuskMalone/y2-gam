using Image;
using System;

namespace Object
{

    public class Button : Entity
    {
        public Vector2 outofRange = new Vector2(999,999);

        public Button() : base()
        {

        }

        public Button(uint entityHandle) : base(entityHandle)
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
                Vector2 buttonEnd = new Vector2(Translation.X, Translation.Y + (Scale.Y / 2.0f));
                if (PhysicsWrapper.Raycast(Collider, buttonEnd, entityID, out RaycastHit buttonHit) && (buttonHit.tag == "Player" || buttonHit.tag == "SwappableBox") )
                {
                    Console.WriteLine("Player touched a button!");

                    Translation = outofRange;
                    Collider = outofRange;
                    
                }
            }
        }

        void OnExit()
        {

        }

    }
}
