/******************************************************************************/
/*!
\par        Image Engine
\file       Main.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Nov 23, 2023

\brief      The main entity class is located here and has the getter setters 
            for all the required rigid body variables. Helper functions also
            located here.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

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

        #region Gameplay
        public void FireCard(Vector2 startPos)
        {
            InternalCalls.GameplayComponent_FireCard(ref startPos);
        }
        public void DestroyEntity(uint entID)
        {
            InternalCalls.GameplayComponent_Destroy(ref entID);
        }
        #endregion

        #region EngineCore
        /*  _________________________________________________________________________ */
        /*! MousePos

        Gets the current mouse pos.
        */
        public Vector2 MousePos
        {
            get
            {
                InternalCalls.EngineCore_GetMousePos(out Vector2 mousePos);
                return mousePos;
            }
        }

        /*  _________________________________________________________________________ */
        /*! PlayAudio

        Loads the specified scene.
        */
        public void PlayAudio(string audioFileName, int loopCount)
        {
            InternalCalls.EngineCore_PlayAudio(out audioFileName, out loopCount);
        }

        /*  _________________________________________________________________________ */
        /*! LoadScene

        Loads the specified scene.
        */
        public void LoadScene(string sceneName)
        {
            InternalCalls.EngineCore_LoadScene(out sceneName);
        }

        /*  _________________________________________________________________________ */
        /*! IsEditorMode
        
        Get the editor mode.
        */
        public bool IsEditorMode()
        {
            InternalCalls.EngineCore_IsEditorMode(out bool isEditorMode);
            return isEditorMode;
        }

        /*  _________________________________________________________________________ */
        /*! SetText
        
        Set the text.
        */
        public void SetText(string text)
        {
            InternalCalls.EngineCore_SetText(entityID, out text);
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

        #region Serialization
        public bool FacingDirection
        {
            get
            {
                InternalCalls.SerializationComponent_GetIsFacingRight(entityID, out bool facingDirection);
                return facingDirection;
            }
            set
            {
                InternalCalls.SerializationComponent_SetIsFacingRight(entityID, ref value);
            }
        }
        #endregion

        #region UI
        public bool UIClicked
        {
            get
            {
                InternalCalls.UIComponent_GetIsUIButtonClicked(entityID, out bool outIsClicked);
                return outIsClicked;
            }
        }

        public bool UIHover
        {
            get
            {
                InternalCalls.UIComponent_GetIsUIButtonHover(entityID, out bool outIsHover);
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
                InternalCalls.AnimationComponent_GetAnimationState(entityID, out int animationState);
                return AnimationState;
            }
            set
            {
                InternalCalls.AnimationComponent_SetAnimationState(entityID, ref value);
            }
        }

        public void SetSprite(string fileName)
        {
            InternalCalls.GraphicsComponent_SetSprite(entityID, out fileName);
        }

        /*  _________________________________________________________________________ */
        /*! Scale

        Getter setter for Scale.
        */
        public Vector3 Scale
        {
            get
            {
                InternalCalls.GraphicsComponent_GetScale(entityID, out Vector3 scale);
                return scale;
            }
            set
            {
                InternalCalls.GraphicsComponent_SetScale(entityID, ref value);
            }
        }

        public Vector4 Colour
        {
            set
            {
                InternalCalls.GraphicsComponent_SetColour(entityID, ref value);
            }
        }
        #endregion

        #region Physics
        /*  _________________________________________________________________________ */
        /*! Collider

        Getter setter for Collider.
        */
        public Vector2 Collider
        {
            get
            {
                InternalCalls.PhysicsComponent_GetColliderPos(entityID, out Vector2 pos);
                return pos;
            }
            set
            {
                InternalCalls.PhysicsComponent_SetColliderPos(entityID, ref value);
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
                InternalCalls.TransformComponent_GetTranslation(entityID, out Vector2 translation);
                return translation;
            }
            set
            {
                InternalCalls.TransformComponent_SetTranslation(entityID, ref value);
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
                InternalCalls.ForceComponent_GetForce(entityID, out Vector2 force);
                return force;
            }
            set
            {
                InternalCalls.ForceComponent_SetForce(entityID, ref value);
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
                InternalCalls.ForceComponent_GetMass(entityID, out float mass);
                return mass;
            }
            set
            {
                InternalCalls.ForceComponent_SetMass(entityID, ref value);
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
                InternalCalls.ForceComponent_GetVelocity(entityID, out Vector2 velocity);
                return velocity;
            }
            set
            {
                InternalCalls.ForceComponent_SetVelocity(entityID, ref value);
            }
        }
        #endregion
    }
}
