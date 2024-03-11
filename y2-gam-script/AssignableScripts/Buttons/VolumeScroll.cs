using Image;
using System;

namespace Object
{
    public class VolumeScroll : Entity
    {
        public float LowerXBound;
        public float UpperXBound;
        public float YBound;

        public string MusicType;

        public VolumeScroll() : base()
        {

        }

        public VolumeScroll(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        void OnCreate()
        {
            // Start at 50%
            Translation = new Vector2(LowerXBound + (UpperXBound - LowerXBound) / 2.0f, YBound);
        }

        void OnUpdate(float dt)
        {
            if (UIClicked && Input.IsMousePressed(KeyCode.MOUSE_BUTTON_LEFT))
            {
                if (MousePosUI.X > LowerXBound && MousePosUI.X < UpperXBound)
                {
                    Translation = new Vector2(MousePosUI.X, YBound);

                    // Linearly Interpolate the scroll position with volume
                    SetAudioGroupVolume(MusicType, Easing.Remap(Translation.X, LowerXBound, UpperXBound, 0.0f, 1.0f));

                    if (MusicType == "sfx")
                    {
                        // Play sample sfx
                        PlayAudio("Card-Throw_SFX_1.wav", 0);
                    }
                }
            }
        }

        void OnExit()
        {

        }
    }
}
