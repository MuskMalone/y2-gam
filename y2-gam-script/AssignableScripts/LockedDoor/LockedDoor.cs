using Image;
using System;

namespace Object
{
    public class LockedDoor : Entity
    {
        Player player = GameplayWrapper.FindEntityByName("Player").As<Player>();
        private bool isUnlocked = false;
        private float doorColliderYOffset = 300.0f;

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
                (player.Collider.Y <= Translation.Y + Range) && (player.Collider.Y >= Translation.Y - Range) && player.KeyCollected && !isUnlocked)
            {
                Console.WriteLine("Door Unlocked");
                player.KeyCollected = false;
                isUnlocked = true;
                Collider = new Vector2(Collider.X, Collider.Y - doorColliderYOffset);
            }
        }

        void OnExit()
        {

        }

    }

}