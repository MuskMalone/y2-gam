/******************************************************************************/
/*!
\par        Image Engine
\file       Main.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Dec 26, 2023

\brief      The main entity class is located here and has the getter setters 
            for all the required rigid body variables. Helper functions also
            located here.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using System;
using System.Runtime.CompilerServices;

namespace Image
{
    public class Entity
    {
        public uint entityID;

        /*  _________________________________________________________________________ */
        /*! Entity
        
        @param entityHandle
        The entityID.

        @return *this

        Non-default, single-arg constructor for entity
        */
        public Entity(uint entityHandle)
        {     
            entityID = entityHandle;
        }

        /*  _________________________________________________________________________ */
        /*! Entity

        @return *this

        Default constructor for entity.
        */
        public Entity()
        {
            entityID = 0;
        }

        /*  _________________________________________________________________________ */
        /*! As

        @return T

        For turning any generic Entity into its respective child class.
        */
        public T As<T>() where T : Entity, new()
        {
            object instance = InternalCalls.EngineCore_GetScriptInstance(ref entityID);
            return instance as T;
        }

        #region EngineCore
        /*  _________________________________________________________________________ */
        /*! MousePos

        Gets the current mouse pos.
        */
        public Vector2 MousePos
        {
            get
            {
                Vector2 mousePos = new Vector2();
                InternalCalls.EngineCore_GetMousePos(ref mousePos);
                return mousePos;
            }
        }

        /*  _________________________________________________________________________ */
        /*! MousePosUI

        Gets the current mouse pos.
        */
        public Vector2 MousePosUI
        {
            get
            {
                Vector2 mousePos = new Vector2();
                InternalCalls.EngineCore_GetUIMousePos(ref mousePos);
                return mousePos;
            }
        }

        /*  _________________________________________________________________________ */
        /*! PlayAudio

        Plays any sound file loaded in the asset manager. Remember to add '.wav' to
        the end of the audioFileName
        */
        public void PlayAudio(string audioFileName, int loopCount)
        {
            InternalCalls.EngineCore_PlayAudio(audioFileName, ref loopCount);
        }

        /*  _________________________________________________________________________ */
        /*! StopAudio

        Stops the current bgm.
        */
        public void StopAudio()
        {
            InternalCalls.EngineCore_StopAudio();
        }

        /*  _________________________________________________________________________ */
        /*! StopAudioFromFilename

        Stops audio channel, given its filename.
        */
        public void StopAudioWithFilename(string audioFileName)
        {
            InternalCalls.EngineCore_StopAudioWithFilename(audioFileName);
        }

        /*  _________________________________________________________________________ */
        /*! ResumeAudioFromFilename

        Resumes audio channel, given its filename.
        */
        public void ResumeAudioWithFilename(string audioFileName)
        {
            InternalCalls.EngineCore_ResumeAudioWithFilename(audioFileName);
        }

        /*  _________________________________________________________________________ */
        /*! PauseAudioFromFilename

        Pauses audio channel, given its filename.
        */
        public void PauseAudioWithFilename(string audioFileName)
        {
            InternalCalls.EngineCore_PauseAudioWithFilename(audioFileName);
        }

        /*  _________________________________________________________________________ */
        /*! LoadScene

        Loads the specified scene.
        */
        public void LoadScene(string sceneName)
        {
            InternalCalls.EngineCore_LoadScene(sceneName);
        }

        /*  _________________________________________________________________________ */
        /*! SaveScene

        Saves the specified scene.
        */
        public void SaveScene(string sceneName)
        {
            InternalCalls.EngineCore_SaveScene(sceneName);
        }

        /*  _________________________________________________________________________ */
        /*! GetCurrentScene

        Loads the specified scene.
        */
        public string GetCurrentScene()
        {
            return (string)InternalCalls.EngineCore_GetCurrentScene(); ;
        }
            

        /*  _________________________________________________________________________ */
        /*! IsEditorMode
        
        Get the editor mode.
        */
        public bool IsEditorMode()
        {
            bool isEditorMode = false;
            InternalCalls.EngineCore_IsEditorMode(ref isEditorMode);
            return isEditorMode;
        }

        /*  _________________________________________________________________________ */
        /*! SetText
        
        Set the text.
        */
        public void SetText(string text)
        {
            InternalCalls.EngineCore_SetText(ref entityID, text);
        }

        /*  _________________________________________________________________________ */
        /*! Quit
        
        Quits the game.
        */
        public void QuitGame()
        {
            InternalCalls.EngineCore_Quit();
        }
        #endregion

        #region UI
        public bool UIClicked
        {
            get
            {
                bool outIsClicked = false;
                InternalCalls.UIComponent_GetIsUIButtonClicked(ref entityID, ref outIsClicked );
                return outIsClicked;
            }
        }

        public bool UIHover
        {
            get
            {
                bool outIsHover = false;
                InternalCalls.UIComponent_GetIsUIButtonHover(ref entityID, ref outIsHover);
                return outIsHover;
            }
        }
        #endregion

        #region Graphics
        /*  _________________________________________________________________________ */
        /*! AnimationState

        Getter setter for AnimationState.
        */
        public int AnimationState
        {
            get
            {
                int animationState = 0;
                InternalCalls.AnimationComponent_GetAnimationState(ref entityID, ref animationState);
                return AnimationState;
            }
            set
            {
                InternalCalls.AnimationComponent_SetAnimationState(ref entityID, ref value);
            }
        }

        /*  _________________________________________________________________________ */
        /*! SetSprite

        Wrapper function for setting the sprite based on the sprite's filename.
        */
        public void SetSprite(string fileName)
        {
            InternalCalls.GraphicsComponent_SetSprite(ref entityID, fileName);
        }

        /*  _________________________________________________________________________ */
        /*! Scale

        Getter setter for Scale.
        */
        public Vector3 Scale
        {
            get
            {
                Vector3 scale = new Vector3();
                InternalCalls.GraphicsComponent_GetScale(ref entityID, ref scale);
                return scale;
            }
            set
            {
                InternalCalls.GraphicsComponent_SetScale(ref entityID, ref value);
            }
        }

        /*  _________________________________________________________________________ */
        /*! Rotation

        Getter setter for Rotation.
        */
        public Vector3 Rotation
        {
            get
            {
                Vector3 rotation = new Vector3();
                InternalCalls.GraphicsComponent_GetRotation(ref entityID, ref rotation );
                return rotation;
            }
            set
            {
                InternalCalls.GraphicsComponent_SetRotation(ref entityID, ref value);
            }
        }

        /*  _________________________________________________________________________ */
        /*! GetScaleFromEntity

        Get the scale, given the entity id.
        */
        public Vector3 GetScaleFromEntity(uint id)
        {
            Vector3 scale = new Vector3();
            InternalCalls.GraphicsComponent_GetScale(ref id, ref scale);
            return scale;
        }

        /*  _________________________________________________________________________ */
        /*! SetScaleFromEntity

        Set the scale, given the entity id.
        */
        public void SetScaleFromEntity(uint id, Vector3 value)
        {
            InternalCalls.GraphicsComponent_SetScale(ref id, ref value);
        }

        /*  _________________________________________________________________________ */
        /*! Colour

        Setter for Colour.
        */
        public Vector4 Colour
        {
            set
            {
                InternalCalls.GraphicsComponent_SetColour(ref entityID, ref value);
            }
        }

        /*  _________________________________________________________________________ */
        /*! SetEntityColour

        Sets a particular entity's colour.
        */
        public void SetEntityColour(uint id, Vector4 col)
        {
            InternalCalls.GraphicsComponent_SetColour(ref id, ref col);
        }

        #endregion

        #region Physics
        /*  _________________________________________________________________________ */
        /*! ColliderDimensions

        Getter setter for Collider.
        */
        public Vector2 ColliderDimensions
        {
            
            get{
                Vector2 dim = new Vector2();
                InternalCalls.PhysicsComponent_GetColliderDimensions(ref entityID, ref dim);
                return dim;
            }

            set
            {
                InternalCalls.PhysicsComponent_SetColliderDimensions(ref entityID, ref value);
            }
        }

        /*  _________________________________________________________________________ */
        /*! Collider

        Getter setter for Collider.
        */
        public Vector2 Collider
        {
            get
            {
                Vector2 pos = new Vector2();
                InternalCalls.PhysicsComponent_GetColliderPos(ref entityID, ref pos);
                return pos;
            }
            set
            {
                InternalCalls.PhysicsComponent_SetColliderPos(ref entityID, ref value);
            }
        }

        /*  _________________________________________________________________________ */
        /*! Translation

        Getter setter for Translation.
        */
        public Vector2 Translation
        {
            get
            {
                Vector2 translation = new Vector2();
                InternalCalls.TransformComponent_GetTranslation(ref entityID, ref translation);
                return translation;
            }
            set
            {
                InternalCalls.TransformComponent_SetTranslation(ref entityID, ref value);
            }
        }
        public float Transform_Rotation
        {
            get
            {
                float rot = 0;
                InternalCalls.TransformComponent_GetRotation(ref entityID, ref rot);
                return rot;
            }
            set
            {
                InternalCalls.TransformComponent_SetRotation(ref entityID, ref value);
            }
        }

        /*  _________________________________________________________________________ */
        /*! Force

        Getter setter for Force.
        */
        public Vector2 Force
        {
            get
            {
                Vector2 force = new Vector2();
                InternalCalls.ForceComponent_GetForce(ref entityID, ref force );
                return force;
            }
            set
            {
                InternalCalls.ForceComponent_SetForce(ref entityID, ref value);
            }
        }

        
        public Vector2 Gravity
        {
            set
            {
                InternalCalls.ForceComponent_SetGravity(ref entityID, ref value);
            }
        }
        

        /*  _________________________________________________________________________ */
        /*! Mass

        Getter setter for Mass.
        */
        public float Mass
        {
            get
            {
                float mass = 0.0f;
                InternalCalls.ForceComponent_GetMass(ref entityID, ref mass);
                return mass;
            }
            set
            {
                InternalCalls.ForceComponent_SetMass(ref entityID, ref value);
            }
        }

        /*  _________________________________________________________________________ */
        /*! Velocity

        Getter setter for Velocity.
        */
        public Vector2 Velocity
        {
            get
            {
                Vector2 velocity = new Vector2();
                InternalCalls.ForceComponent_GetVelocity(ref entityID, ref velocity);
                return velocity;
            }
            set
            {
                InternalCalls.ForceComponent_SetVelocity(ref entityID, ref value);
            }
        }
        #endregion
    }
}
