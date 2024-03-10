

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
        //2sec to fade in 
        private float Panel2Timer = 1.0f;
        //the time the last panel left off with
        private float lastPanelTime = 6.0f;
        //leave as zero
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
                    //Console.WriteLine("Panel2Time AFT2: " + PanelTime);
                }
                else if (PanelTime < Panel2Timer)
                {
                    //Console.WriteLine("Panel2Time: " + PanelTime);
                    PanelTime += dt; // Update the panel time

                    // Calculate the exact color value based on elapsed time, scaled to the range 0 to 1
                    normalizeTime = PanelTime / Panel2Timer;
                    ColourValue = normalizeTime * normalizeTime;
                    // Ensure the value does not exceed 1
                    ColourValue = Math.Min(ColourValue, 1.0f);

                    //Console.WriteLine("ColourValue: " + ColourValue);
                    // Update the colour with the new value
                    Colour = new Vector4(1, 1, 1, ColourValue); 
                }

                //FADEOUT
                if (cutscenep1.Panel1Time >= cutscenep1.FadeOutBegins && cutscenep1.CutsceneIndex == 0)
                {
                    Console.WriteLine("Reached end of 23sec");
                    cutscenep1.FadeOutTime += dt;
                    if (cutscenep1.FadeOutTime <= cutscenep1.FadeOutTimer)
                    {
                        float normalizedFadeOutTime = cutscenep1.FadeOutTime / cutscenep1.FadeOutTimer;
                        float easeOut = normalizedFadeOutTime * (2 - normalizedFadeOutTime);
                        ColourValue = 1.0f - easeOut;
                        ColourValue = Math.Max(ColourValue, 0.0f);
                        Console.WriteLine("Colour Value: " + ColourValue);
                        Colour = new Vector4(1, 1, 1, ColourValue);
                    }
                    //FINISH FADEOUT
                    else
                    {
                        Console.WriteLine("Entered end fadeout");
                        Colour = new Vector4(1, 1, 1, 0.0f);
                        cutscenep1.CutsceneIndex++;
                    }
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