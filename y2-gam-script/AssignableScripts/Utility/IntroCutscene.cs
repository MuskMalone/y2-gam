

using Image;
using System;

namespace Object
{
    public class IntroCutscene : Entity
    {

        public int CutsceneIndex = 0;
        public int LastCutscene = 4;
        public int CurrPanel = 1;
        public float PanelTime = 0.0f;
        //fade out
        //Counter for fade out
        public float FadeOutTime = 0.0f;
        //Max duration of fade out
        public float FadeOutTimer = 1.0f;
        //The Panel1Time in which fade out begins
        public float FadeOutBegins = 24.3f;

        //Amount of time to fade in
        private float PanelFadeInTimer = 1.0f;
        private float normalizeTime = 0.0f;
        //Variable to hold opacity value 
        private float ColourValue = 0.0f;

        public string cutsceneTag;
        IntroCutscene introCutscene;
        //Time for each panels
        //private float Panel1Time = 6.0f;
        //private float Panel2Time = 9.5f;
        //private float Panel3Time = 14.7f;

        /*  _________________________________________________________________________ */
        /*! Cutscene

        @return *this

        Default constructor for the Cutscene entity. Inherits from entity class.
        */
        public IntroCutscene() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Cutscene
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a Cutscene entity.
        */
        public IntroCutscene(uint entityHandle) : base(entityHandle)
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
            if (cutsceneTag != null)
            {
                introCutscene = GameplayWrapper.FindEntityByName(cutsceneTag).As<IntroCutscene>();
            }
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
        //    if (CutsceneIndex == 0)
        //    {
        //        FadeOutBegins = 24.3f;
        //        if (PanelTime < 6.0f)
        //        {
        //            CurrPanel = 1;
        //            cutsceneTag = "c0p1";
        //        }
        //        else if (PanelTime < 9.5f)
        //        {
        //            CurrPanel = 2;
        //            cutsceneTag = "c0p2";
        //        }
        //        //else if (PanelTime < 14.7f)
        //        //{
        //        //    CurrPanel = 3;
        //        //}
        //        //else
        //        //{
        //        //    CurrPanel = 4;
        //        //}
        //    }

        //    if(PanelTime < FadeOutBegins)
        //    {
        //        FadeIn(dt);
        //    }
        //    else
        //    {
        //        FadeOut(dt);
        //    }

        //    if (CutsceneIndex >= LastCutscene)
        //    {
        //        LoadScene("LevelSelect"); // Load the next scene if the last cutscene is reached
        //    }
        }

        void FadeIn(float dt)
        {
            //Time for audio to finish, after fade in, before fade out
            if ((PanelTime >= PanelFadeInTimer) && CutsceneIndex == 0)
            {
                introCutscene.Colour = new Vector4(1, 1, 1, 1);
                //Update the panel time
                PanelTime += dt;
                Console.WriteLine("Panel1Time AFT2: " + PanelTime);

            }
            //Fading in 
            else if ((PanelTime < PanelFadeInTimer) && CutsceneIndex == 0)
            {
                //Console.WriteLine("Panel1Time: " + PanelTime);
                PanelTime += dt; // Update the panel time

                // Calculate the exact color value based on elapsed time, scaled to the range 0 to 1
                normalizeTime = PanelTime / PanelFadeInTimer;
                ColourValue = normalizeTime * normalizeTime;
                ColourValue = Math.Min(ColourValue, 1.0f); // Ensure the value does not exceed 1

                //Console.WriteLine("ColourValue: " + ColourValue);
                introCutscene.Colour = new Vector4(1, 1, 1, ColourValue); // Update the colour with the new value
            }
        }

        void FadeOut(float dt)
        {
            //FADEOUT
            if (CutsceneIndex == 0)
            {
                Console.WriteLine("Reached end of 23sec");
                FadeOutTime += dt;
                if (FadeOutTime <= FadeOutTimer)
                {
                    float normalizedFadeOutTime = FadeOutTime / FadeOutTimer;
                    float easeOut = normalizedFadeOutTime * (2 - normalizedFadeOutTime);
                    ColourValue = 1.0f - easeOut;
                    ColourValue = Math.Max(ColourValue, 0.0f);
                    Console.WriteLine("Colour Value: " + ColourValue);
                    introCutscene.Colour = new Vector4(1, 1, 1, ColourValue);
                }
                //FINISH FADEOUT
                else
                {
                    introCutscene.Colour = new Vector4(1, 1, 1, 0.0f);

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

