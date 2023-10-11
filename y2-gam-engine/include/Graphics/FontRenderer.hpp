/******************************************************************************/
/*!
\par        Image Engine
\file       FontRenderer.hpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Sep 17, 2023

\brief      Header file for font rendering.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#pragma once

#include "Graphics/Shader.hpp"
#include "Graphics/Renderer.hpp"
#include "Components/Camera.hpp"
#include <Core/Globals.hpp>

#include "Math/MathUtils.h"

#include <ft2build.h>
#include FT_FREETYPE_H

constexpr int MAX_ASCII{ 128 };
constexpr unsigned int MAXQUADS{ 1000 };
constexpr unsigned int MAXVERTICES{ MAXQUADS * 4 };
constexpr unsigned int MAXINDICES{ MAXQUADS * 6 };

namespace Image {
  struct FontVtx {
    Vec2 pos;
    Vec2 texCoord;
  };

  struct Character {
    float uMin;
    float uMax;
    float vMin;
    float vMax;
    glm::ivec2   size;
    glm::ivec2   bearing; // Offset from baseline to left/top of glyph
    unsigned int advance; // Offset to advance to next glyph
  };

  struct FaceObject {
    FT_Face face{};
    std::shared_ptr<VertexArray> vao;
    std::shared_ptr<VertexBuffer> vbo;
    std::shared_ptr<ElementBuffer> ebo;
  };

  class FontRenderer {
  public:
    static void Init();
    static void Exit();
    static void LoadFont(const char * filepath, std::string name);
    static void GenerateBitmap(std::string name, int fontSize);
    static void SetFontSize(std::string name, int fontSize);
    static void RenderText(std::string fontname, std::string text, 
      float xPos, float yPos, float scale, Vec3 color);

  private:
    static void DebugGlyph(std::string name, int width, int height, unsigned char* buffer);
    static void CreateGLObjects(std::string name);

  private:
    static FT_Library sLib;
    static std::map<std::string, FaceObject> sFaces;
    static std::map<std::string, std::map<char, Character>> sCharacters;
    static std::map<std::string, std::shared_ptr<Texture>> sBitmap;
    //static Shader* sShaderPgm;
    static std::shared_ptr<Shader> sShaderPgm;
  };
}