/******************************************************************************/
/*!
\par        Image Engine
\file       Globals.hpp

\author     tan cheng hian (t.chenghian)
\date       Sep 17, 2023

\brief      globals + data tweaking

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once
#include <limits>
#include <map>
#include <vector>
#include <fstream>
#include <memory>
#include <DataMgmt/DecisionTree/DecisionTree.hpp>


namespace Globals {
    struct GlobalValue {
        const int val;
        template <typename T>
        operator T() { return static_cast<T>(val); }
        GlobalValue operator-() { return GlobalValue{ -val }; }
        template<typename T>
        T operator-(T const& r) { return static_cast<T>(val) - r; }
        template<typename T>
        T operator+(T const& r) { return static_cast<T>(val) + r; }
        template<typename T>
        T operator/(T const& r) { return static_cast<T>(val) / r; }
        template<typename T>
        T operator*(T const& r) { return static_cast<T>(val) * r; }
    };
    template<typename T>
    T operator-(T const& r, GlobalValue const& l) { return static_cast<T>(l.val) - r; }
    template<typename T>
    T operator+(T const& r, GlobalValue const& l) { return static_cast<T>(l.val) + r; }
    template<typename T>
    T operator/(T const& r, GlobalValue const& l) { return static_cast<T>(l.val) / r; }
    template<typename T>
    T operator*(T const& r, GlobalValue const& l) { return static_cast<T>(l.val) * r; }

    class GlobalValContainer {
    public:
        static std::shared_ptr<GlobalValContainer> GetInstance() {
            if (!_mSelf) _mSelf = std::make_shared<GlobalValContainer>();
            return _mSelf;
        }
        void ReadGlobalInts() {
            std::ifstream ifsGlobals{"Data/Globals.ini"};
            if (!ifsGlobals.is_open()) exit(1);
            char ch;
            do {
                char i = static_cast<char>(ifsGlobals.peek());
                if (std::isdigit(i)) {
                    int k;
                    ifsGlobals >> k;
                    mGlobalInts.push_back(GlobalValue{ k });
                }
            } while (ifsGlobals >> ch);
            ifsGlobals.close();
        }
        GlobalValue At(int i) {
            return mGlobalInts[i];
        }
    private:
        static std::shared_ptr<GlobalValContainer> _mSelf;
        std::vector<GlobalValue> mGlobalInts;
    };

};
#define FLOAT_MAX (std::numeric_limits<float>::max())

#define UNREFERENCED_PARAMETER(P) (P)

#ifndef _INSTALLER
  #define WINDOW_TITLE "Image Engine"
#else
  #define WINDOW_TITLE "Blink"
#endif

//i saw in glfw3.h that the max GLenum goes up to ~380ish
//hence this is gonna be for a bitset
//each key enum will be the bitshift for a bitset<ENGINE_KEYS_COUNT>
#define ENGINE_KEYS_COUNT 1000
#define ENGINE_MOUSEKEYS_COUNT 3
#define ENGINE_PHYSICS_PROFILE 0
#define ENGINE_COLLISION_PROFILE 1
#define ENGINE_RENDER_PROFILE 2
#define ENGINE_INPUT_PROFILE 3
#define ENGINE_GUI_PROFILE 4

//from here on are serialized globals
#define GVC_AT(I) Globals::GlobalValContainer::GetInstance()->At(I)
//world center is at 0,0
#define WORLD_LIMIT_X GVC_AT(0)//128
#define WORLD_LIMIT_Y GVC_AT(1)//128
//depth is 0 to 256
#define WORLD_LIMIT_DEPTH GVC_AT(2)//256

#define ENGINE_SCREEN_WIDTH GVC_AT(3)//1600
#define ENGINE_SCREEN_HEIGHT GVC_AT(4)//900

#define IMGUI_MASS GVC_AT(5)//10
#define IMGUI_SCALE GVC_AT(6)//15
#define IMGUI_GRAVITY GVC_AT(7)//10
#define IMGUI_MAX_SCALE GVC_AT(8)//100
#define IMGUI_MAX_GRAVITY GVC_AT(9)//100
#define IMGUI_MAX_SPEED_ANIM GVC_AT(10)//1
#define IMGUI_MAX_VELOCITY GVC_AT(11)//100
#define IMGUI_MAX_FRICTION GVC_AT(12)//100
#define CAMERA_MOVESPEED GVC_AT(13)//100
#define CAMERA_ROTSPEED GVC_AT(14)//80
#define CAMERA_ZOOMSPEED GVC_AT(15)//1

//extern DecisionTree gGameLoop;