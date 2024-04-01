#include "QF_ShaderFactory.h"

// #TODO Deprecate or move and 1 line with documentation

namespace QwerkE {

    std::string ShaderFactory::Basic2DShaderVert(std::string vertString)
    {
        /* Header */
        vertString.append("// Basic2D.vert");
        // SetVersion(vertString);

        // Input
        AddInputVec3("Position", vertString);

        // Output

        /* Body */
        OpenMain(vertString);

        vertString.append("\ngl_Position = vec4(t_Position, 1.0);");

        CloseMain(vertString);

        return vertString;
    }

    std::string ShaderFactory::Basic2DShaderFrag(std::string fragString)
    {
        /* Header */
        fragString.append("// Basic2D.frag");
        // SetVersion(fragString);

        // Input

        // Output

        /* Body */
        OpenMain(fragString); // end header / begin body
        fragString.append("\ngl_FragColor = vec4(1,1,1,1);"); // TODO: Add color?

        CloseMain(fragString);

        return fragString;
    }

}
