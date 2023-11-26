using Image;
using System;

namespace Object
{

    public class Button : Entity
    {
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
                Vector2 buttonEnd = new Vector2(Translation.X, Translation.Y + (Scale.Y/2.0f));
                if (PhysicsWrapper.Raycast(Collider, buttonEnd, entityID, out RaycastHit buttonHit) && buttonHit.tag == "Player")
                {
                    Console.WriteLine("Player touched a button!");
                    //Colour = new Vector4(1, 1, 1, 1);
                    //SetSprite("gam200_anvil");
                }
            }
        }

        void OnExit()
        {

        }

    }
}
