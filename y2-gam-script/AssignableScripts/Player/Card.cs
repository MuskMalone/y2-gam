/******************************************************************************/
/*!
\par        Image Engine
\file       Card.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Nov 23, 2023

\brief      Script for the card. Includes the UI card.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using Image;
using System;
using System.Collections.Generic;

namespace Object
{
    public class Card : Entity
    {
        private Vector2 direction;
        private float timeAlive = 0.0f;
        private bool Alive = false;

        private readonly float MAX_TIME_ALIVE = 3.5f;
        private readonly float speed = 45.0f;

        private uint CardUIID;
        private Vector3 CardUIMaxScale;

        private bool firstTime = true;

        private uint HoveredID;
        private bool _isHovered;
        public bool Hovering
        {
            get { return _isHovered; }
            set
            {
                if (_isHovered != value)
                {
                   _isHovered = value;
                    HoveringChanged = true; // Set another flag when isFacingRight changes
                }
            }
        }

        Vector2 temp_pos;
        Vector2 temp_Force;
        Vector2 temp_velocity;
        float temp_dt = 0f;
        bool isPaused = false;
        public bool HoveringChanged { get; private set; }

        private List<string> CardSwapAudio = new List<string>();
        private List<string> CardThrowAudio = new List<string>();
        private int CardSwapAudioCounter = 0;
        private int CardThrowAudioCounter = 0;
        private readonly int MAX_AUDIO_FILES = 2;

        /*  _________________________________________________________________________ */
        /*! Card

        @return *this

        Default constructor for the card entity. Inherits from entity class.
        */
        public Card() : base()
        {

        }

        /*  _________________________________________________________________________ */
        /*! Card
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for a card entity.
        */
        public Card(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! OnCreate

        @return none

        Called on creation of the card entity.
        */
        void OnCreate()
        {
            Hovering = false;
            ResetCardPos();

            CardSwapAudio.Add("Card-Swap_SFX_1.wav");
            CardSwapAudio.Add("Card-Swap_SFX_2.wav");

            CardThrowAudio.Add("Card-Throw_SFX_1.wav");
            CardThrowAudio.Add("Card-Throw_SFX_2.wav");
        }

        /*  _________________________________________________________________________ */
        /*! OnUpdate
        
        @param dt
        Delta time.

        @return none

        Called on every update for the card entity.
        */
        void OnUpdate(float dt)
        {
            if (firstTime)
            {
                CardUIID = GameplayWrapper.GetIDFromTag("CardUI");
                CardUIMaxScale = GetScaleFromEntity(CardUIID);
                firstTime = false;
            }

            if (!IsEditorMode())
            {
                if (isPaused)
                {
                    dt = temp_dt;
                    PauseGame();
                    //AnimationState = temp_AnimationState;
                }

                if (Input.IsKeyClicked(KeyCode.KEY_ESCAPE))
                {
                    if (!isPaused)
                    {
                        PauseGame();
                        temp_dt = dt;
                        dt = temp_dt;
                        isPaused = true;
                    }
                    else
                    {
                        //resume game
                        ResumeGame();
                        //dt = temp_dt;
                        isPaused = false;
                    }
                    //firstTime = false;
                }
                if (!isPaused)
                {
                    if (Alive)
                    {
                        if (Input.IsMouseClicked(KeyCode.MOUSE_BUTTON_RIGHT))
                        {
                            PlayAudio("out_of_cards.wav", 0);
                        }

                        // Swap Related
                        if (Input.IsMouseClicked(KeyCode.MOUSE_BUTTON_LEFT))
                        {
                            if (PhysicsWrapper.Raycast(MousePos, MousePos, entityID, out RaycastHit swapRayCast))
                            {
                                if (GameplayWrapper.IsSwappable(swapRayCast.id))
                                {
                                    ResetCardUI();
                                    GameplayWrapper.Swap(entityID, swapRayCast.id);

                                    CardSwapAudioCounter++;
                                    if (CardSwapAudioCounter >= MAX_AUDIO_FILES)
                                    {
                                        CardSwapAudioCounter = 0;
                                    }
                                    PlayAudio(CardSwapAudio[CardSwapAudioCounter], 0);

                                    ResetCardPos();
                                    ResetColour(swapRayCast.id);
                                }
                            }
                        }

                        // Check for Hover
                        if (PhysicsWrapper.Raycast(MousePos, MousePos, entityID, out RaycastHit mouseRayCast))
                        {
                            Hovering = true;

                            if (HoveredID != mouseRayCast.id)
                            {
                                ResetColour(HoveredID);
                            }

                            HoveredID = mouseRayCast.id;

                            if (GameplayWrapper.IsSwappable(mouseRayCast.id))
                            {
                                SetEntityColour(mouseRayCast.id, new Vector4(0, 1, 0, 1));
                            }

                            else
                            {
                                SetEntityColour(mouseRayCast.id, new Vector4(1, 0, 0, 1));
                            }
                        }

                        else
                        {
                            Hovering = false;
                        }

                        if (HoveringChanged && !Hovering)
                        {
                            ResetColour(HoveredID);
                        }

                        /*
                        if (PhysicsWrapper.IsCollidedWithAnything(entityID))
                        {
                            ResetCardPos();
                        }
                        */

                        // Card Related
                        timeAlive += dt;
                        Velocity += direction * speed * dt;

                        SetEntityColour(CardUIID,
                            new Vector4(1, 1, 1, (float)ScaleToRange(timeAlive, 0, MAX_TIME_ALIVE, 0, 1)));

                        SetScaleFromEntity(CardUIID, new Vector3(
                            (float)ScaleToRange(timeAlive, 0, MAX_TIME_ALIVE, 0, CardUIMaxScale.X),
                            (float)ScaleToRange(timeAlive, 0, MAX_TIME_ALIVE, 0, CardUIMaxScale.Y), 1));

                        if ((timeAlive >= MAX_TIME_ALIVE))
                        {
                            ResetCardPos();
                            ResetColour(HoveredID);
                            ResetCardUI();
                        }
                    }

                    else
                    {
                        if (Input.IsMouseClicked(KeyCode.MOUSE_BUTTON_RIGHT))
                        {
                            FireCard();
                        }
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

        void PauseGame()
        {
            //pause the game
            temp_Force = Force;
            temp_pos = Translation;
            temp_velocity = Velocity;
            //temp_AnimationState = AnimationState;
            Force = new Vector2(0, 0);
            Translation = new Vector2((float)temp_pos.X, (float)temp_pos.Y);
            Velocity = new Vector2(0, 0);
            //AnimationState = temp_AnimationState;
        }

        void ResumeGame()
        {
            Force = temp_Force * temp_dt;
            Translation = temp_pos;
            Velocity = temp_velocity * temp_dt;
            //AnimationState = temp_AnimationState;
        }
        void ResetColour(uint id)
        {
            SetEntityColour(id, new Vector4(1, 1, 1, 1));
        }

        void ResetCardUI()
        {
            Console.WriteLine("Reset Card UI");
            SetEntityColour(CardUIID, new Vector4(1, 1, 1, 1));
            SetScaleFromEntity(CardUIID, new Vector3(CardUIMaxScale.X, CardUIMaxScale.Y, 1));
        }

        void ResetCardPos()
        {
            Translation = new Vector2(9999, 9999);
            Collider = new Vector2(9999, 9999);
            Colour = new Vector4(1, 1, 1, 0);
            timeAlive = 0.0f;
            Alive = false;
            Velocity = new Vector2(0, 0);
        }

        void FireCard()
        {
            //Console.WriteLine("Card Fired");
            //Console.WriteLine("Player Position" + GameplayWrapper.PlayerPos.X + ", " + GameplayWrapper.PlayerPos.Y);
            Translation = GameplayWrapper.PlayerPos;
            Collider = GameplayWrapper.PlayerPos;
            direction = MousePos - GameplayWrapper.PlayerPos;
            direction = PhysicsWrapper.Normalize(direction);
            Colour = new Vector4(1, 1, 1, 1);
            Alive = true;

            CardThrowAudioCounter++;
            if (CardThrowAudioCounter >= MAX_AUDIO_FILES)
            {
                CardThrowAudioCounter = 0;
            }
            PlayAudio(CardThrowAudio[CardThrowAudioCounter], 0);
        }

        static double ScaleToRange(double value, double oldMin, double oldMax, double newMin, double newMax)
        {
            if (oldMin == oldMax)
                return newMin;

            double normalizedValue = (value - oldMin) / (oldMax - oldMin);
            double scaledValue = normalizedValue * (newMax - newMin) + newMin;

            return Math.Min(Math.Max(scaledValue, newMin), newMax);
        }
    }
}
