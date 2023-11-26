using Image;
using System;

namespace Object
{

    public class Button : Entity
    {
        //public bool created = false;

        public Button() : base()
        {

        }

        public Button(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        void OnCreate()
        {
            //GameplayWrapper.IsPressed = false;
            //GameplayWrapper.IsPressed = false;
        }



        void OnUpdate(float dt)
        {
            if (!IsEditorMode())
            {

                Vector2 buttonEnd = new Vector2(Translation.X, Translation.Y + (Scale.Y / 2.0f));
                if (PhysicsWrapper.Raycast(Collider, buttonEnd, entityID, out RaycastHit buttonHit) && (buttonHit.tag == "Player" || buttonHit.tag == "SwappableBox"))
                {
                    Console.WriteLine("Player touched a button!");
                    //if (created == false)
                    //{
                    GameplayWrapper.SpawnPrefab("OpenDoor", new Vector2(100, -33));
                        //created = true;
                    //}
                    //GameplayWrapper.IsPressed = true;

                }
               
            }
        }

        void OnExit()
        {

        }

    }
}
