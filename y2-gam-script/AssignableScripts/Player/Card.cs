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
        Player player = GameplayWrapper.FindEntityByName("Player").As<Player>();
        Arms arms; 

        private Vector2 direction;
        public float timeAlive = 0.0f;
        public bool Alive = false;

        public float MAX_TIME_ALIVE;
        public float speed;

        public uint CardUIID;
        public Vector3 CardUIMaxScale;

        private bool firstTime = true;
        public float CardReleaseDelay;
        public float CardReleaseTime = 0.0f;
        private bool StartDelay = false;

        private uint HoveredID;
        private bool _isHovered;

        private bool firstTimeEnterDeath = true;

        private Vector2 PositionToFire;
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

        float temp_dt = 0f;
        public bool HoveringChanged { get; private set; }

        private List<string> CardSwapAudio = new List<string>();
        private List<string> CardThrowAudio = new List<string>();
        private int CardSwapAudioCounter = 0;
        private int CardThrowAudioCounter = 0;
        private readonly int MAX_AUDIO_FILES = 2;

        private ReverbCode reverbSetting;

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
            Alive = false;

            CardSwapAudio.Add("Card-Swap_SFX_1.wav");
            CardSwapAudio.Add("Card-Swap_SFX_2.wav");

            CardThrowAudio.Add("Card-Throw_SFX_1.wav");
            CardThrowAudio.Add("Card-Throw_SFX_2.wav");

            String currentScene = GetCurrentScene();

            if (currentScene == "Level1")
            {
                reverbSetting = ReverbCode.OFF;
            }

            else if (currentScene == "Level1Transition")
            {
                reverbSetting = ReverbCode.OFF;
            }

            else if (currentScene == "Level2")
            {
                reverbSetting = ReverbCode.CAVE;
            }
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
                Velocity = new Vector2(0.0f, 0.0f);
                CardUIID = GameplayWrapper.GetIDFromTag("CardUI");
                CardUIMaxScale = GetScaleFromEntity(CardUIID);
                firstTime = false;
                ResetCardPos();
                arms = GameplayWrapper.FindEntityByName("Arms").As<Arms>();
            }

            if (player.isPaused)
            {
                dt = 0f;
                //PauseGame();
            }

            if (!player.isPaused)
            {
                if (Alive)
                {
                    if (Input.IsMouseClicked(KeyCode.MOUSE_BUTTON_RIGHT))
                    {
                        PlayAudio("out_of_cards.wav", 0, (int)reverbSetting);
                    }

                    // Card Related (Add time and velocity when alive)
                    timeAlive += dt;
                    Velocity += direction * speed * dt;

                    SetEntityColour(CardUIID,
                        new Vector4(1, 1, 1, (float)ScaleToRange(timeAlive, 0, MAX_TIME_ALIVE, 0, 1)));

                    SetScaleFromEntity(CardUIID, new Vector3(
                        (float)ScaleToRange(timeAlive, 0, MAX_TIME_ALIVE, 0, CardUIMaxScale.X),
                        (float)ScaleToRange(timeAlive, 0, MAX_TIME_ALIVE, 0, CardUIMaxScale.Y), 1));

                    if ((timeAlive >= MAX_TIME_ALIVE) || PhysicsWrapper.IsCollidedWithAnything(entityID))
                    {
                        ResetCardPos();
                        ResetCardUI();
                        return;
                    }

                    // Swap Related
                    if(Input.IsKeyClicked(KeyCode.KEY_Q))
                    {
                        if (!PhysicsWrapper.IsIntersectedWithLayer(entityID, "NoSwap"))
                        {
                            timeAlive = 0.0f;
                            ResetCardUI();
                            GameplayWrapper.Swap(entityID, player.entityID);
                            player.PlayAppearAnimation = true;
                            CardSwapAudioCounter++;
                            if (CardSwapAudioCounter >= MAX_AUDIO_FILES)
                            {
                                CardSwapAudioCounter = 0;
                            }
                            PlayAudio(CardSwapAudio[CardSwapAudioCounter], 0, (int)reverbSetting);

                            ResetCardPos();
                        }
                    }

                    if (Input.IsMousePressed(KeyCode.MOUSE_BUTTON_LEFT))
                    {
                        if (PhysicsWrapper.Raycast(MousePos, MousePos, entityID, out RaycastHit swapRayCast))
                        {
                            //if (swapRayCast.id != player.entityID)
                            //{
                                if (GameplayWrapper.IsSwappable(swapRayCast.id))
                                {
                                    timeAlive = 0.0f;
                                    ResetCardUI();

                                    GameplayWrapper.Swap(entityID, swapRayCast.id);

                                    CardSwapAudioCounter++;
                                    if (CardSwapAudioCounter >= MAX_AUDIO_FILES)
                                    {
                                        CardSwapAudioCounter = 0;
                                    }
                                    PlayAudio(CardSwapAudio[CardSwapAudioCounter], 0, (int)reverbSetting);

                                    ResetCardPos();
                                    ResetColour(swapRayCast.id);
                                }
                            //}
                        }
                    }

                    // Check for Hover
                    if (PhysicsWrapper.Raycast(MousePos, MousePos, entityID, out RaycastHit mouseRayCast))
                    {
                        if (mouseRayCast.layer != "Platform" && mouseRayCast.layer != "NoSwap" && mouseRayCast.layer != "Spikes")
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
                    }

                    else
                    {
                        Hovering = false;
                    }

                    if (HoveringChanged && !Hovering)
                    {
                        ResetColour(HoveredID);
                    }

                    if (PhysicsWrapper.IsCollidedWithAnything(entityID))
                    {
                        ResetCardPos();
                    }
                }

                else
                {
                    if (firstTimeEnterDeath)
                    {
                        ResetColour(HoveredID);
                        firstTimeEnterDeath = false;
                    }
                        
                    if (Input.IsMouseClicked(KeyCode.MOUSE_BUTTON_RIGHT) && !StartDelay)
                    {
                        StartDelay = true;
                        PositionToFire = MousePos;
                    }

                    if (StartDelay)
                    {
                        CardReleaseTime += dt;

                        if (CardReleaseTime >= CardReleaseDelay)
                        {
                            firstTimeEnterDeath = true;
                            CardReleaseTime = 0;
                            StartDelay = false;
                            FireCard(PositionToFire);
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

        void ResetColour(uint id)
        {
            SetEntityColour(id, new Vector4(1, 1, 1, 1));
        }

        public void ResetCardUI()
        {
            SetEntityColour(CardUIID, new Vector4(1, 1, 1, 1));
            SetScaleFromEntity(CardUIID, new Vector3(CardUIMaxScale.X, CardUIMaxScale.Y, 1));
        }

        public void ResetCardPos()
        {
            Alive = false;
            Velocity = new Vector2(0, 0);         
            Translation = new Vector2(99999, 99999);
            Collider = new Vector2(99999, 99999);
            Colour = new Vector4(1, 1, 1, 0);
            timeAlive = 0.0f; 
        }

        void FireCard(Vector2 mousePos)
        {
            //float cardOffset = (player.IsFacingRight) ? player.CardThrowOffsetFromPlayer : -player.CardThrowOffsetFromPlayer;
            /*
            Translation = new Vector2(GameplayWrapper.PlayerPos.X + cardOffset, GameplayWrapper.PlayerPos.Y);
            Collider = new Vector2(GameplayWrapper.PlayerPos.X + cardOffset, GameplayWrapper.PlayerPos.Y);
            */
            Translation = arms.Translation;
            Collider = Translation;
            direction = mousePos - Translation;
            direction = PhysicsWrapper.Normalize(direction);
            Colour = new Vector4(1, 1, 1, 1);
            Alive = true;

            CardThrowAudioCounter++;
            if (CardThrowAudioCounter >= MAX_AUDIO_FILES)
            {
                CardThrowAudioCounter = 0;
            }
            PlayAudio(CardThrowAudio[CardThrowAudioCounter], 0, (int)reverbSetting);
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
