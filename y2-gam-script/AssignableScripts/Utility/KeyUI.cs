
using Image;
using System;

namespace Object
{
    public class KeyUI : Entity
    {
        bool firsttime = true;
        public string keyTag;
        Player player;
        Key key;
        /*  _________________________________________________________________________ */
        /*! ZoomOut

        @return *this

        Default constructor for the ZoomOut entity. Inherits from entity class.
        */
        public KeyUI() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! ZoomOut
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a ZoomOut entity.
        */
        public KeyUI(uint entityHandle) : base(entityHandle)
        {
            key = GameplayWrapper.FindEntityByName("Key").As<Key>();
            player = GameplayWrapper.FindEntityByName("Player").As<Player>();
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation.
        */

        void OnCreate()
        {
            Colour = new Vector4(1, 0, 1, 0);
        }

        /*  _________________________________________________________________________ */
        /*! OnUpdate
        
        @param dt
        Delta time.

        @return none

        Called on every update.
        */
        void OnUpdate(float dt)
        {
            //if(keyTag == "purple" )
            //{
            //    if(player.KeyCollected && key.keyTag == "purple")
            //    {
            //        Colour = new Vector4(1, 1, 1, 1);
                   
            //    }
            //    //else if(!player.KeyCollected)
            //    //{
            //    //    Colour = new Vector4(1, 1, 1, 0);
            //    //}
            //}
            //else if(keyTag == "gold" )
            //{
            //    //Console.WriteLine(keyTag);
            //    if (player.KeyCollected && key.keyTag == "gold")
            //    {
            //        Colour = new Vector4(1, 1, 1, 1);

            //    }
            //    //else if (!player.KeyCollected)
            //    //{
            //    //    Colour = new Vector4(1, 1, 1, 0);
            //    //}
            //}
            //else
            //{
            //    Colour = new Vector4(1, 1, 1, 0);
            //}
            if(player.KeyCollected)
            {
                if(key.keyTag == "purple" && keyTag == "purple")
                {
                    if (keyTag == "purple")
                    {
                        Console.WriteLine(key.keyTag);
                        Console.WriteLine(keyTag);
                        Colour = new Vector4(1, 1, 1, 1);
                    }
                    else
                    {
                        Colour = new Vector4(1, 1, 1, 0);
                    }
                }
            }

        }

        /*  _________________________________________________________________________ */
        /*! OnExit

        @return none

        Called on exit.
        */
        void OnExit()
        {

        }

    }
}
