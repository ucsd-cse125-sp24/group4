// #pragma once

// #include <ft2build.h>
// #include FT_FREETYPE_H  

// #include <unordered_map>
// #include <glm/glm.hpp>
// #include "glad/glad.h"
// #include <GLFW/glfw3.h>
// #include <string>

// namespace gui::font {

//     struct Character {
//         unsigned int textureID;  // ID handle of the glyph texture
//         glm::ivec2 size;         // Size of the glyph
//         glm::ivec2 bearing;      // Offset from baseline to left/top of the glyph
//         unsigned int advance;    // Horizontal offset to advance to next glyph
//     };

//     class FontRenderer {
//     public:
//         bool init();
//         const Character& loadChar(char c) const;
//         ~FontRenderer();

//     private:
//         FT_Library ft;
//         FT_Face face;
//         static constexpr int UNIT_LARGE_SIZE_PX = 48; // Adjust size as needed
//         bool loadFont();
//         Character defaultCharacter;
//     };

// }
