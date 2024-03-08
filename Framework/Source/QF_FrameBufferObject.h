#pragma once

#ifdef _QGLEW
#include "Libraries/glew/glew.h"
#endif

#include "QF_Renderer.h"

namespace QwerkE {

    class FrameBufferObject
    {
    public:
        void Init()
        {
            // #TODO Review and remove unnecessary comments
            const vec2& windowSize = Renderer::WindowSize();

#ifdef _QGLEW
            glGenFramebuffers(1, &m_FramebufferHandle);
            glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle);

            glGenTextures(1, &m_TextureColorbuffer);
            glBindTexture(GL_TEXTURE_2D, m_TextureColorbuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (int)windowSize.x, (int)windowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureColorbuffer, 0);

            glGenRenderbuffers(1, &m_Rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, m_Rbo);

            // Use a single render buffer object for both a depth AND stencil buffer.
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (int)windowSize.x, (int)windowSize.y);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_Rbo); // Now attach the render buffer

            // Now that we actually created the frame buffer and added all attachments, we want to check if it is actually complete now
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                // cout << "ERROR::FRAMEBUFFER:: Frame buffer is not complete!" << endl; // #TODO Review error handling
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
        }

        void Bind()
        {
#ifdef _QGLEW
            glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferHandle);
#endif
        }

        void UnBind()
        {
#ifdef _QGLEW
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
        }

        void Reset() { m_TextureColorbuffer = 0; }; // #TODO De-allocate GPU data?

#ifdef _QGLEW
        GLuint GetTextureID() { return m_TextureColorbuffer; };

    private:
        GLuint m_FramebufferHandle = 0;
        GLuint m_TextureColorbuffer = 0;
        GLuint m_Rbo = 0;
#endif
    };

}
