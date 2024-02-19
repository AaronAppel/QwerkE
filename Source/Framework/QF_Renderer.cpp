#include "QF_Renderer.h"

#include "QC_Vector.h"

#include "QF_Debug.h"
#include "QF_FontTest.h"
#include "QF_Graphics_Header.h"
#include "QF_Log.h"
#include "QF_Renderer.h"
#include "QF_Resources.h"
#include "QF_ShaderProgram.h"

namespace QwerkE {

    namespace Renderer {

        vec2 s_WindowSize = vec2(1600, 900);

        // TODO: Move OpenGL code to proper files
        void Initialize()
        {
            FT_Library freeType;
            if (FT_Init_FreeType(&freeType))
            {
                ASSERT(false, "Error loading freetype2!");
            }
            else
            {
                LOG_TRACE("Freetype Loaded,");
                FT_Done_FreeType(freeType);
            }

            LoadFonts();

            // TODO: GL state init should be in a Window() or OpenGLManager()
            //     class or some type of ::Graphics() system.
            glClearColor(0.5f, 0.7f, 0.7f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // turn on depth buffer testing
            glEnable(GL_DEPTH_TEST);
            glPointSize(10);
            glLineWidth(10);

            // depth cull for efficiency
            // TODO: This is also in the framework
            // glEnable(GL_CULL_FACE);
            // glDisable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            if (Wind_CCW) glFrontFace(GL_CCW);
            else glFrontFace(GL_CW);

            // turn on alpha blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glViewport(0, 0, s_WindowSize.x, s_WindowSize.y);
        }

        void NewFrame()
        {
            ClearScreen();
        }

        void ClearScreen()
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void DrawFont(const char* text, float x, float y, float scale)
        {
            // TODO: Improve font rendering
            RenderText(Resources::GetShaderProgram("font_shader.ssch"),
                std::string(text),
                x, y, scale,
                glm::vec3(0, 0, 0));
        }

        const vec2& WindowSize()
        {
            return s_WindowSize;
        }

    }

}
