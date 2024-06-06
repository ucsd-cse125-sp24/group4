// #include "../include/fonts.h"

// #include <iostream>


// namespace gui::font
// {
//
//     bool FontRenderer::init()
//     {
//         if (FT_Init_FreeType(&ft))
//         {
//             std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
//             return false;
//         }

//         glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

//         if (!loadFont())
//         {
//             return false;
//         }

//         return true;
//     }

//     bool FontRenderer::loadFont()
//     {
//         auto path = "./include/fonts/fonts.ttf"

//         std::cout << "Loading font: " << path << "\n";

//         if (FT_New_Face(ft, path.c_str(), 0, &face))
//         {
//             std::cout << "ERROR::FREETYPE: Failed to load font at " << path << std::endl;
//             return false;
//         }

//         FT_Set_Pixel_Sizes(face, 0, UNIT_LARGE_SIZE_PX);

//         return true;
//     }

//     const Character &FontRenderer::loadChar(char c) const
//     {
//         if (FT_Load_Char(face, c, FT_LOAD_RENDER))
//         {
//             std::cerr << "ERROR::FREETYTPE: Failed to load Glyph " << c << std::endl;
//             return defaultCharacter; // Return a default character or handle the error appropriately
//         }

//         unsigned int texture;
//         glGenTextures(1, &texture);
//         glBindTexture(GL_TEXTURE_2D, texture);
//         glTexImage2D(
//             GL_TEXTURE_2D,
//             0,
//             GL_RED,
//             face->glyph->bitmap.width,
//             face->glyph->bitmap.rows,
//             0,
//             GL_RED,
//             GL_UNSIGNED_BYTE,
//             face->glyph->bitmap.buffer);

//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//         Character character = {
//             texture,
//             glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
//             glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
//             static_cast<unsigned int>(face->glyph->advance.x)};

//         return character;
//     }

//     FontRenderer::~FontRenderer()
//     {
//         FT_Done_Face(face);
//         FT_Done_FreeType(ft);
//     }

// }