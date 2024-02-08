using Image;
using System;

namespace Object
{
    public class Key : Entity
    {
        Player player = GameplayWrapper.FindEntityByName("Player").As<Player>();
        public float Range;
        //public bool Collected = false;
        public Key() : base()
        {

        }

        public Key(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        void OnCreate()
        {

        }

        void OnUpdate(float dt)
        {
            if ((player.Collider.X <= Translation.X + Range) && (player.Collider.X >= Translation.X - Range) &&
                (player.Collider.Y <= Translation.Y + Range) && (player.Collider.Y >= Translation.Y - Range) && !player.KeyCollected)
            {
                Console.WriteLine("COLLECTEDDDDDDDDDDDDDDDDDDDDDDDD");
                player.KeyCollected = true;
            }
        }

        void OnExit()
        {

        }

    }

}