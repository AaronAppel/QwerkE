#include "QF_FontTest.h"

#include <iostream>
#include <map>

#include "Libraries/freetype2/ft2build.h"
#include "Libraries/freetype2/freetype/freetype.h"
#include "Libraries/glew/GL/glew.h"
#pragma warning( disable : 26495 )
#include "Libraries/glm/glm/common.hpp"
#include "Libraries/glm/glm/gtc/type_ptr.hpp"
#include "Libraries/glm/glm/gtc/matrix_transform.hpp"
#pragma warning( default : 26495 )

#include "QC_StringHelpers.h"

#include "QF_Defines.h"
#include "QF_Graphics_Header.h"
#include "QF_GraphicsHelpers.h"
#include "QF_Log.h"
#include "QF_Renderer.h"
#include "QF_Assets.h"
#include "QF_ShaderProgram.h"

// Copied from font rendering example : https://learnopengl.com/

namespace QwerkE {

    // Holds all state information relevant to a character as loaded using FreeType
    struct Character {
        GLuint TextureID;   // ID handle of the glyph texture
        glm::ivec2 Size;    // Size of glyph
        glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
        GLuint Advance;    // Horizontal offset to advance to next glyph
    };

    std::map<GLchar, Character> Characters;
    GLuint VAO, VBO;

    void LoadFonts()
    {
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

        // Load font as face
        FT_Face face;
        // #TODO Change freetype font loading to use Resources().
        if (FT_New_Face(ft, FontsFolderPath(null_font), 0, &face))
        {
            LOG_ERROR("{0} Freetype failed to load font {1}!", __FUNCTION__, FontsFolderPath(null_font));
            return;
        }

        // Set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // Disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Load first 128 characters of ASCII set
        for (GLubyte charGlyph = 0; charGlyph < 128; charGlyph++)
        {
            if (FT_Load_Char(face, charGlyph, FT_LOAD_RENDER))
            {
                LOG_ERROR("{0} Freetype failed to load character glyph {1}!", __FUNCTION__, charGlyph);
                continue;
            }

            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                (GLuint)face->glyph->advance.x
            };

            Characters.insert(std::pair<GLchar, Character>(charGlyph, character));
        }

        // Destroy FreeType once we're finished	FT_Done_Face(face);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Configure VAO/VBO for texture quads
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        Assets::AddFont(null_font, face);
    }

    void RenderText(ShaderProgram* shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
    {
        // https://learnopengl.com/In-Practice/Text-Rendering
        const vec2& windowSize = Renderer::WindowSize();
        glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(windowSize.x), 0.0f, static_cast<GLfloat>(windowSize.y));

        shader->Use();
        // glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        shader->SetUniformMat4("projection", glm::value_ptr(projection));
        // Activate corresponding render state
        // glUniform3f(glGetUniformLocation(shader->GetProgram(), "textColor"), color.x, color.y, color.z);
        shader->SetUniformFloat3("textColor", color.x, color.y, color.z);

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // Iterate through all characters
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = Characters[*c];

            GLfloat xpos = x + ch.Bearing.x * scale;
            GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            GLfloat w = ch.Size.x * scale;
            GLfloat h = ch.Size.y * scale;
            // Update VBO for each character
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
            };

            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);

            CheckGraphicsErrors(__FILE__, __LINE__); // DEBUG:
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            CheckGraphicsErrors(__FILE__, __LINE__); // DEBUG:
            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6)* scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))

            CheckGraphicsErrors(__FILE__, __LINE__); // DEBUG:
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        CheckGraphicsErrors(__FILE__, __LINE__); // DEBUG:
    }

}
