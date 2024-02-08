using Image;
using System;

namespace Object
{
    public class LockedDoor : Entity
    {
        Player player = GameplayWrapper.FindEntityByName("Player").As<Player>();

        public float Range;

        public LockedDoor() : base()
        {

        }

        public LockedDoor(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        void OnCreate()
        {

        }

        void OnUpdate(float dt)
        {
            if ((player.Collider.X <= Translation.X + Range) && (player.Collider.X >= Translation.X - Range) &&
                (player.Collider.Y <= Translation.Y + Range) && (player.Collider.Y >= Translation.Y - Range) && player.KeyCollected)
            {
                Console.WriteLine("UNLOCKEDDDDDDD");
                player.KeyCollected = false;
            }
        }

        void OnExit()
        {

        }

    }

}