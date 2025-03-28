#include "QF_ShaderUtilities.h"

#include "QF_ShaderComponent.h"
#include "QF_ShaderProgram.h"

namespace QwerkE {

    void FindShaderUniformsAndAttributesInComponentStrings(ShaderProgram* shader, std::vector<std::string>& attributes, std::vector<std::string>& uniforms)
    {
        // https://stackoverflow.com/questions/440144/in-opengl-is-there-a-way-to-get-a-list-of-all-uniforms-attribs-used-by-a-shade

        // TODO: Improve logic. Each shader may have both attributes and uniforms that may be declared
        // more than once total. Handle in 2 stages, all attributes then all uniforms.

        if (!shader) return;

        attributes.clear();
        uniforms.clear();

        if (shader->GetVertShader())
        {
            // store string in temp vertexBuffer
            char* buffer = (char*)DeepCopyString(shader->GetVertShader()->GetStringData()); // Delete vertexBuffer when done

            // Store lines in std::vector
            char* next_token = 0;
            char* line = strtok_s(buffer, "\n", &next_token);

            std::vector<std::string> vertStringList;

            while (line)
            {
                //OutputMessage("%s\n", line);
                vertStringList.push_back(line);
                line = strtok_s(0, "\n", &next_token);
            }
            free(buffer); // cleanup

            /* Populate .vert attributes */
            // vert
            for (unsigned int i = 0; i < vertStringList.size(); i++) // stringList.size() = number of lines in file
            {
                std::string loopString = vertStringList.at(i);
                if (loopString.at(0) == 'i' && loopString.find("in") != loopString.npos) // starts with 'i' and has "in" in line
                {
                    unsigned int size = 20;
                    std::string t_Variable;
                    sscanf_s((char*)loopString.c_str(), "%*s %*s %s", (char*)t_Variable.c_str(), size);
                    strtok_s((char*)t_Variable.c_str(), ";", &next_token); // remove ';' from end
                    // #TODO Fix C6031 warning "Return value ignored: 'strtok_s'."

                    std::string loopString;
                    int counter = 2; // remove 'u_' from beginning
                    while (t_Variable[counter] != '\0')
                    {
                        loopString.push_back(t_Variable[counter]);
                        counter++;
                    }

                    attributes.push_back(loopString);
                }
                // duplicates caught in .vert by compiler
            }
        }

        if (shader->GetFragShader() && shader->GetVertShader()) // #TODO Review logic for why vertex data is used here
        {
            // vert
            // store string in temp vertexBuffer
            char* vertexBuffer = (char*)DeepCopyString(shader->GetVertShader()->GetStringData());

            // store lines in std::vector
            char* next_token = 0;
            char* line = strtok_s(vertexBuffer, "\n", &next_token);

            std::vector<std::string> vertStringList;
            std::vector<std::string> fragStringList;
            // TODO: Geo shader uniforms

            while (line)
            {
                //OutputMessage("%s\n", line);
                vertStringList.push_back(line);
                line = strtok_s(0, "\n", &next_token);
            }
            free(vertexBuffer);

            // frag
            char* fragmentBuffer = (char*)DeepCopyString(shader->GetFragShader()->GetStringData());
            line = strtok_s(fragmentBuffer, "\n", &next_token);

            while (line)
            {
                //OutputMessage("%s\n", line);
                fragStringList.push_back(line);
                line = strtok_s(0, "\n", &next_token);
            }
            free(fragmentBuffer);

            /* Populate .vert uniforms */
            for (unsigned int i = 0; i < vertStringList.size(); i++) // stringList.size() = number of lines in file
            {
                std::string loopString = vertStringList.at(i);
                if (loopString.at(0) == 'u' && loopString.find("uniform") != loopString.npos) // starts with 'u' and has uniform in line
                {
                    unsigned int size = 20;
                    std::string t_Variable;
                    sscanf_s((char*)loopString.c_str(), "%*s %*s %s", (char*)t_Variable.c_str(), size);
                    strtok_s((char*)t_Variable.c_str(), ";", &next_token); // remove ';' from end
                    // #TODO Fix C6031 warning "Return value ignored: 'strtok_s'."

                    std::string loopString;
                    int counter = 2; // remove 'u_' from beginning
                    while (t_Variable[counter] != '\0')
                    {
                        loopString.push_back(t_Variable[counter]);
                        counter++;
                    }
                    // TODO: Check for duplicates? Caught by compiler?
                    uniforms.push_back(loopString);
                }
            }
            /* Populate .frag uniforms */
            for (unsigned int i = 0; i < fragStringList.size(); i++) // stringList.size() = number of lines in file
            {
                std::string loopString = fragStringList.at(i);
                if (loopString.at(0) == 'u' && loopString.find("uniform") != loopString.npos)
                {
                    unsigned int size = 20;
                    std::string t_Variable;
                    sscanf_s((char*)loopString.c_str(), "%*s %*s %s", (char*)t_Variable.c_str(), size);
                    strtok_s((char*)t_Variable.c_str(), ";", &next_token); // remove ';' from end
                    // #TODO Fix C6031 warning "Return value ignored: 'strtok_s'."

                    std::string loopString;
                    int counter = 2; // remove 'u_' from beginning
                    while (t_Variable[counter] != '\0')
                    {
                        loopString.push_back(t_Variable[counter]);
                        counter++;
                    }
                    // TODO: Check for duplicates? Caught by compiler?
                    uniforms.push_back(loopString);
                }
            }
        }

        // if (shader->GetGeoShader())
        {
            // TODO:
        }
    }

}
