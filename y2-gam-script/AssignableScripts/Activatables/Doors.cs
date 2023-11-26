using Image;
using System;

namespace Object
{

    public class Button : Entity
    {
        private bool isClicked = false;
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
                if (PhysicsWrapper.Raycast(Collider, buttonEnd, entityID, out RaycastHit buttonHit) && (buttonHit.tag == "Player" || buttonHit.tag == "SwappableBox") && !isClicked)
                {
                    Console.WriteLine("Player touched a button!");
                    GameplayWrapper.SpawnPrefab("OpenDoor", new Vector2(100, -33));
                    isClicked = true;
                    
                }
            }
        }

        void OnExit()
        {

        }

    }
}
