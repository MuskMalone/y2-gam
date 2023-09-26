/******************************************************************************/
/*!
\par        Image Engine
\file       FontRenderer.cpp

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Sep 17, 2023

\brief      Source file for font rendering.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.h"

//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include <stb_image_write.h>

#include "Graphics/FontRenderer.hpp"
#include "Core/Coordinator.hpp"
#include "Systems/RenderSystem.hpp"

namespace {
  std::shared_ptr<Coordinator> gCoordinator;
}

namespace Image {

  // Static Initialization
  FT_Library FontRenderer::sLib{ NULL };
  std::map<std::string, FaceObject> FontRenderer::sFaces{};
  std::map<std::string, std::map<char, Character>> FontRenderer::sCharacters{};
  std::map<std::string, std::shared_ptr<Texture>> FontRenderer::sBitmap{};
  //Shader* FontRenderer::sShaderPgm{nullptr};
  std::shared_ptr<Shader> FontRenderer::sShaderPgm{ nullptr };

  /*  _________________________________________________________________________ */
  /*! Init

  @return none.

  Initializes the FreeType library and Shaders.
  */
  void FontRenderer::Init() {
    if (FT_Init_FreeType(&sLib)) {
      std::cout << "Could not init FreeType Library" << "\n";
      return;
    }
    else {
      std::cout << "Successful init FreeType Library" << "\n";
    }

    //Shader sh("../Shaders/fontVertex.glsl", "../Shaders/fontFragment.glsl");
    sShaderPgm = std::make_shared<Shader>("../Shaders/fontVertex.glsl", "../Shaders/fontFragment.glsl");
    //sShaderPgm = &sh;
  }

  /*  _________________________________________________________________________ */
  /*! Exit

  @return none.

  Releases memory used by FreeType.
  */
  void FontRenderer::Exit() {
    for (auto& i : sFaces) {
      FaceObject& curr = i.second;
      FT_Done_Face(curr.face);
    }

    FT_Done_FreeType(sLib);
  }

  /*  _________________________________________________________________________ */
  /*! LoadFont

  @param filepath
  The filepath containing the true type font file.

  @param name
  Name of the font. Will be used later.

  @return none.

  Creates the font face based on the true type font file.
  */
  void FontRenderer::LoadFont(const char* filepath, std::string name) {
    
    FaceObject fo{};
    FT_Face addFace;
    FT_Error result{ FT_New_Face(sLib, filepath, 0, &addFace) };

    if (result) {
      std::cout << "Failed to add font face for " << name << "\n";
      return;
    }
    else {
      std::cout << "Successful init FreeType Face " << name << "\n";
    }

    fo.face = addFace;
    sFaces.insert(std::pair<std::string, FaceObject>(name, fo));
  }

  /*  _________________________________________________________________________ */
  /*! DebugGlyph

  @param glyphBitmap
  The individual glyph.

  @param ch
  The character.

  @param name
  Name of the font.

  @return none.

  Exports glypth to png files for debugging purposes.
  */
  void FontRenderer::DebugGlyph(std::string name, int width, int height, unsigned char* buffer) {
    // Define the output file path (e.g., based on character's Unicode value).
    std::string outputPath = "../Glyphs/" + name + ".png";

    // Save the glyph image as a PNG file.
    //stbi_write_png(outputPath.c_str(), width, height, 4, buffer, 0);
  }

  /*  _________________________________________________________________________ */
  /*! SetFontSize

  @param name
  Name of the font.

  @param fontSize
  Font size of the font to set to.

  @return none.

  Sets the font size of a loaded font.
  */
  void FontRenderer::SetFontSize(std::string name, int fontSize) {
    if (sFaces.find(name) == sFaces.end()) {
      std::cout << "Font " << name << " does not exist" << "\n";
      std::exit(EXIT_FAILURE);
    }

    FT_Error result{ FT_Set_Pixel_Sizes(sFaces[name].face, 0, fontSize) };

    if (result) {
      std::cout << "Failed to Set Font Size for " << name << "\n";
      return;
    }
    else {
      std::cout << "Successful Set Font Size to " << fontSize << "\n";
    }
  }

  /*  _________________________________________________________________________ */
  /*! GenerateBitmap

  @param name
  The name of the font.

  @param fontSize
  The font size of the font.

  @return none.

  Generates a bitmap font for the entire font (basically a giant texture for 
  all the characters in a font). 
  */
  void FontRenderer::GenerateBitmap(std::string name, int fontSize) {
    if (sFaces.find(name) == sFaces.end()) {
      std::cout << "Font " << name << " does not exist" << "\n";
      std::exit(EXIT_FAILURE);
    }

    FT_UInt glyphIndex{};
    FT_Error error{};
    FT_Face const& currFace{ sFaces[name].face };

    // size of the bitmap
    int imageWidth{ (fontSize + 6) * 16 };
    int imageHeight{ (fontSize + 2) * 8 };

    // create a buffer for the bitmap (multiply by 4 for colors)
    unsigned char* buffer = new unsigned char[imageWidth * imageHeight * 4] {};

    // create an array to save the character widths
    int* widths = new int[MAX_ASCII];
    int* heights = new int[MAX_ASCII];

    // draw characters
    for (int ch{}; ch < MAX_ASCII; ++ch) {
      glyphIndex = FT_Get_Char_Index(currFace, ch);
      error = FT_Load_Glyph(currFace, glyphIndex, FT_LOAD_RENDER);

      if (error) {
        std::cout << "Failed to load Glyph for " << static_cast<char>(ch) <<
          " with Error code: " << error << "\n";
        continue;
      }

      error = FT_Render_Glyph(currFace->glyph, FT_RENDER_MODE_NORMAL);
      if (error) {
        std::cout << "Failed to render glyph, Error code: " << error << std::endl;
      }

      // save the character information
      widths[ch] = currFace->glyph->metrics.width / 64;
      heights[ch] = currFace->glyph->metrics.height / 64;

      // find the tile position where we have to draw the character
      int x{ (ch % 16) * (fontSize + 6) };
      int y{ (ch / 16) * (fontSize + 2) };
      x += 3; // 3 pixel padding from the left side of the tile

      // draw the character
      const FT_Bitmap& bitmap = currFace->glyph->bitmap;
      for (unsigned int xx{}; xx < bitmap.width; ++xx) {
        for (unsigned int yy{}; yy < bitmap.rows; ++yy) {
          unsigned char r = bitmap.buffer[(yy * (bitmap.width) + xx)];
          buffer[(y + yy) * imageWidth * 4 + (x + xx) * 4 + 0] = r;
          buffer[(y + yy) * imageWidth * 4 + (x + xx) * 4 + 1] = r;
          buffer[(y + yy) * imageWidth * 4 + (x + xx) * 4 + 2] = r;
          buffer[(y + yy) * imageWidth * 4 + (x + xx) * 4 + 3] = 255;
        }
      }

      Character currChar{};

      currChar.size = { currFace->glyph->bitmap.width, currFace->glyph->bitmap.rows };
      currChar.bearing = { currFace->glyph->bitmap_left, currFace->glyph->bitmap_top };
      currChar.advance = currFace->glyph->advance.x;

      currChar.uMin = static_cast<float>(x) / imageWidth;
      currChar.vMin = static_cast<float>(y) / imageHeight;
      currChar.uMax = currChar.uMin + static_cast<float>(widths[ch]) / imageWidth;
      currChar.vMax = currChar.vMin + static_cast<float>(heights[ch]) / imageHeight;
      

      (sCharacters[name])[ch] = currChar;
    }
/* causing memory leaks
#ifdef _DEBUG
    DebugGlyph(name, imageWidth, imageHeight, buffer);
#endif
*/
    std::shared_ptr<Texture> bitmapTex;
    bitmapTex.reset(new Texture(imageWidth, imageHeight,
      static_cast<const void*>(buffer)));

    sBitmap[name] = bitmapTex;
    CreateGLObjects(name);
  }

  /*  _________________________________________________________________________ */
  /*! CreateGLObjects

  @param name
  Name of the font.

  @return none.

  Creates the vao, vbo and ebo.
  */
  void FontRenderer::CreateGLObjects(std::string name) {
    if (sFaces.find(name) == sFaces.end()) {
      std::cout << "Font " << name << " does not exist" << "\n";
      std::exit(EXIT_FAILURE);
    }

    FaceObject & curr = sFaces[name];

    std::shared_ptr<VertexArray> currVao;
    currVao.reset(new VertexArray);

    std::shared_ptr<VertexBuffer> currVbo;

    // position and texture coordinates
    currVbo.reset(new VertexBuffer(MAXVERTICES * sizeof(float)));

    BufferLayout fontLayout = {
      { AttributeType::VEC2, "a_Position" },
      { AttributeType::VEC2, "a_TexCoord" }
    };

    currVbo->SetLayout(fontLayout);
    currVao->AddVertexBuffer(currVbo);

    unsigned int* quadIndices = new unsigned int[MAXINDICES];
    unsigned int offset{}; 
    for (unsigned int i{}; i < MAXINDICES; i += 6) {
      quadIndices[i + 0] = offset + 0;
      quadIndices[i + 1] = offset + 1;
      quadIndices[i + 2] = offset + 2;

      quadIndices[i + 3] = offset + 2;
      quadIndices[i + 4] = offset + 3;
      quadIndices[i + 5] = offset + 0;

      offset += 4;
    }

    std::shared_ptr<ElementBuffer> currEbo;
    currEbo.reset(new ElementBuffer(quadIndices, MAXINDICES));
    currVao->SetElementBuffer(currEbo);
    
    curr.vao = currVao;
    curr.vbo = currVbo;
    curr.ebo = currEbo;

    delete[] quadIndices;
  }

  /*  _________________________________________________________________________ */
  /*! RenderText

  @param fontname
  Name of the font.

  @param text
  The text to display.

  @param xPos
  X-coordinate on screen.

  @param yPos
  Y-coordinate on screen.

  @param scale
  Scale for the font.

  @param color
  Color of the font.

  @return none.

  The text render function. Loops through the characters in the user passed in
  string, then positions them according to their bearing and size.
  */
  void FontRenderer::RenderText(std::string fontName, std::string text, float xPos, float yPos, 
    float scale, glm::vec3 color) {
    if (sFaces.find(fontName) == sFaces.end()) {
      std::cout << "Font " << fontName << " does not exist" << "\n";
      std::exit(EXIT_FAILURE);
    }

    // Temporary Fix
    yPos = -yPos;

    FaceObject const& currFace{ sFaces[fontName] };
    std::shared_ptr<Texture>const& currTex{ sBitmap[fontName] };

    float worldLimitX{ static_cast<float>(WORLD_LIMIT_X) };
    float worldLimitY{ static_cast<float>(WORLD_LIMIT_Y) };
    //OrthoCamera cam(16/9, -worldLimitX, worldLimitX, worldLimitY, -worldLimitY);
    
    auto& cam{ Coordinator::GetInstance()->GetComponent<OrthoCamera>(Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetCamera()) };
    glm::mat4 projection{ cam.GetProjMtx() };
    glm::mat4 flipY{ glm::mat4(1.0f) };
    flipY[1][1] = -1.0f;
    projection = projection * flipY;

    sShaderPgm->Use();
    sShaderPgm->SetUniform("uTextColor", color.x, color.y, color.z);
    sShaderPgm->SetUniform("uProjection", projection);
    currFace.vao->Bind();
   
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDisable(GL_DEPTH_TEST);

    // update VBO for each character
    std::vector<float> vertices{};
    std::size_t verticesPerCharacter{ static_cast<size_t>(4 * 4) };
    vertices.reserve(verticesPerCharacter * text.length());
    vertices.clear();
    int index{};
    for (char const& ch : text) {
      Character currChar{ (sCharacters[fontName])[ch] };
      
      float xpos{ xPos + currChar.bearing.x * scale };
      float ypos{ yPos - (currChar.size.y + currChar.bearing.y) * scale };

      float width{ currChar.size.x * scale };
      float height{ currChar.size.y * scale };
      
      float arr[4 * 4] = {
        xpos + width, ypos + height, currChar.uMax, currChar.vMin,
        xpos + width, ypos + 2.f * height, currChar.uMax, currChar.vMax,
        xpos, ypos + 2.f * height, currChar.uMin, currChar.vMax,
        xpos, ypos + height, currChar.uMin, currChar.vMin
      };

      auto insertPosition{ vertices.begin() + (index * verticesPerCharacter) };
      vertices.insert(insertPosition, arr, arr + std::size(arr));

      xPos += (currChar.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
      index++;
    }

    // bind the bitmap font texture
    currTex->Bind(0);

    currFace.vbo->Bind();
    currFace.vbo->SetData(vertices.data(), 
      static_cast<unsigned int>(vertices.size() * sizeof(float)));
    currFace.ebo->Bind();
    size_t eboSizePerChar{ 6 };

    Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetFramebuffer()->Bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(eboSizePerChar * text.length()), 
      GL_UNSIGNED_INT, NULL);
    Coordinator::GetInstance()->GetSystem<RenderSystem>()->GetFramebuffer()->Unbind();

    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_BLEND);
    currFace.vao->Unbind();
    currFace.ebo->Unbind();
    currFace.vbo->Unbind();
    sShaderPgm->Unuse();
  }
}