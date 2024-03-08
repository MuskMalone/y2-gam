/******************************************************************************/
/*!
\par        Image Engine
\file       Cutscene.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Feb 8, 2023

\brief      Script for cutscenes.

\copyright  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;
using System;

namespace Object
{
    public class CutsceneP2 : Entity
    {

        //public int CutsceneIndex = 0;
        //public int LastCutscene = 4;
        private float PanelTime = 0.0f;
        CutsceneP1 cutscenep1 = GameplayWrapper.FindEntityByName("CutsceneP1").As<CutsceneP1>();
        //2sec to fade in 6+2
        private float Panel2Timer = 2.0f;
        
        //the time the last panel left off with
        private float lastPanelTime = 6.0f;
        private float normalizeTime = 0.0f;
        private float ColourValue = 0.0f;
        //private int clampedColourValue = 0;

        /*  _________________________________________________________________________ */
        /*! Cutscene

        @return *this

        Default constructor for the Cutscene entity. Inherits from entity class.
        */
        public CutsceneP2() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Cutscene
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a Cutscene entity.
        */
        public CutsceneP2(uint entityHandle) : base(entityHandle)
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
            if (cutscenep1.CutsceneIndex == 0 && cutscenep1.Panel1Time >= lastPanelTime)
            {
                if (PanelTime >= Panel2Timer)
                {
                    Colour = new Vector4(1, 1, 1, 1);
                    PanelTime += dt; // Update the panel time
                    Console.WriteLine("Panel2Time AFT2: " + PanelTime);
                    if (cutscenep1.Panel1Time >= 23.0f)
                    {
                        Console.WriteLine("Reached end of 23sec");
                        //Reset colour to transparent
                        Colour = new Vector4(0, 0, 0, 0);
                        //Reset colour value
                        ColourValue = 0.0f;
                        //Move to the next cutscene
                        cutscenep1.CutsceneIndex++;
                    }
                }
                else if (PanelTime < Panel2Timer)
                {
                    Console.WriteLine("Panel2Time: " + PanelTime);
                    PanelTime += dt; // Update the panel time

                    // Calculate the exact color value based on elapsed time, scaled to the range 0 to 1
                    normalizeTime = PanelTime / Panel2Timer;
                    ColourValue = normalizeTime * normalizeTime;
                    ColourValue = Math.Min(ColourValue, 1.0f); // Ensure the value does not exceed 1

                    Console.WriteLine("ColourValue: " + ColourValue);
                    Colour = new Vector4(1, 1, 1, ColourValue); // Update the colour with the new value
                }
            }
            else
            {
                Colour = new Vector4(1, 1, 1, 0);
            }

            if (cutscenep1.CutsceneIndex >= cutscenep1.LastCutscene)
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