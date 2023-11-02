/******************************************************************************/
/*!
\par        Image Engine
\file       pch.h

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Sep 23, 2023

\brief      The precompiled header file to facilitate faster build times.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

// Pre-compiled headers go here
// Alphabetical order please

#include <algorithm>
#include <array>
#include <bitset>
#include <climits>
#include <chrono>
#include <cstdlib>
#include <ctime> 
#include <condition_variable>
#include <exception>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <random>
#include <unordered_map>
#include <utility>
#include <set>
#include <stack>
#include <string>
#include <stdio.h>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <vector>

#include <stb_image.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Math/MathUtils.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/prettywriter.h>

#include <Logging/backward.hpp>
#include "Logging/LoggingSystem.hpp"

#include <fmod.hpp>
#include <fmod_errors.h>

// Components
#include "Components/Collider.hpp"
#include "Components/Camera.hpp"
#include "Components/Gravity.hpp"
#include "Components/Sprite.hpp"
#include "Components/RigidBody.hpp"
#include "Components/Transform.hpp"
#include "Components/Editor.hpp"
#include "Components/Animation.hpp"
#include "Components/Text.hpp" 
#include "Components/Node.hpp"
#include "Components/Script.hpp"
#include "Components/Tag.hpp"