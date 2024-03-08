﻿
using Image;
using System;

namespace Object
{
    public class CutsceneP1 : Entity
    {

        public int CutsceneIndex = 0;
        public int LastCutscene = 4;
        public float Panel1Time = 0.0f;

        private float Panel1Timer = 1.0f;
        //Panel1Time/255
        //private float Panel1ColourUnit = 7f/255;
        //private float Panel1ColourCounter = 0.0f;
        private float normalizeTime = 0.0f;
        private float ColourValue = 0.0f;
        //private int clampedColourValue = 0;

        /*  _________________________________________________________________________ */
        /*! Cutscene

        @return *this

        Default constructor for the Cutscene entity. Inherits from entity class.
        */
        public CutsceneP1() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Cutscene
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a Cutscene entity.
        */
        public CutsceneP1(uint entityHandle) : base(entityHandle)
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
            if ((Panel1Time >= Panel1Timer) && CutsceneIndex == 0)
            {
                Colour = new Vector4(1, 1, 1, 1);
                Panel1Time += dt; // Update the panel time
                //Console.WriteLine("Panel1Time AFT2: " + PanelTime);
                if (Panel1Time >= 23.0f)
                {
                    //Console.WriteLine("Reached end of 23sec");
                    Colour = new Vector4(0, 0, 0, 0); // Reset colour to transparent
                    ColourValue = 0.0f; // Reset colour value
                    CutsceneIndex++; // Move to the next cutscene
                }
            }
            else if ((Panel1Time < Panel1Timer) && CutsceneIndex == 0)
            {
                //Console.WriteLine("Panel1Time: " + PanelTime);
                Panel1Time += dt; // Update the panel time

                // Calculate the exact color value based on elapsed time, scaled to the range 0 to 1
                normalizeTime = Panel1Time / Panel1Timer;
                ColourValue = normalizeTime * normalizeTime;
                ColourValue = Math.Min(ColourValue, 1.0f); // Ensure the value does not exceed 1

                //Console.WriteLine("ColourValue: " + ColourValue);
                Colour = new Vector4(1, 1, 1, ColourValue); // Update the colour with the new value
            }

            if (CutsceneIndex >= LastCutscene)
            {
                LoadScene("LevelSelect"); // Load the next scene if the last cutscene is reached
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