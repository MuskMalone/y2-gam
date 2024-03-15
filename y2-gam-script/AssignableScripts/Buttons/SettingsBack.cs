﻿using Image;

namespace Object
{
    public class SettingsBack : Entity
    {
        SettingsPage settingsPage;
        public SettingsBack() : base()
        {

        }

        public SettingsBack(uint entityHandle) : base(entityHandle)
        {
            entityID = entityHandle;
        }

        void OnCreate()
        {
            settingsPage = GameplayWrapper.FindEntityByName("Page").As<SettingsPage>();
        }

        void OnUpdate(float dt)
        {
            if (UIHover)
            {
                settingsPage.AnimationState = 1;
            }
            else
            {
                settingsPage.AnimationState = 0;
            }

            if (UIClicked)
            {
                LoadScene(InternalCalls.EngineCore_GetPrevSceneVar());
                PlayAudio("menu_click.wav", 0);
            }
        }

        void OnExit()
        {

        }
    }
}

