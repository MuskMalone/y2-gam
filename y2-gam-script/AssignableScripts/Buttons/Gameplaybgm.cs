﻿using Image;
using System;

namespace Object
{
    public class Gameplaybgm : Entity
    {
        bool firstTime = true;
        bool isPlaying = false;
        /*  _________________________________________________________________________ */
        /*! EnterLevelOne

        @return *this

        Default constructor for the EnterLevelOne entity. Inherits from entity class.
        */
        public Gameplaybgm() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! EnterLevelOne
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a EnterLevelOne entity.
        */
        public Gameplaybgm(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation.
        */
        void OnCreate()
        {

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
            if (GetCurrentScene() == "Level1" && !isPlaying)
            {
                //PlayAudio("Poltergeist'sMansionTheme_BGM_Loop.wav", -1);
                firstTime = true;
                isPlaying = true;
            }
            //else if(firstTime && GetCurrentScene() != "Level1")
            //{
            //    StopAudio();
            //    firstTime = false;
            //}

            if (firstTime)
            {
                StopAudio();
                firstTime = false;
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
