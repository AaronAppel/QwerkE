#include "QF_Shader.h"

#include "QF_Files.h"
#include "QF_Paths.h"

namespace QwerkE {

	// #TODO Review string helpers to add to QC_StringHelpers.h
	// void FindStringInString();
	// void CountStringsInString();
	// void CountUniqueStringsInStringWithPrefix();

	void Shader::ValidateVaryings(const char* a_VertexFilePath, const char* a_FragmentFilePath)
	{
		// #TODO Helper for Triaging shader errors
		ASSERT(a_VertexFilePath && a_VertexFilePath, "Invalid arguments!");

		std::string vertexFilePath = Paths::Shader(Files::FileNameNoExt(a_VertexFilePath).string().c_str());
		if (Files::Exists(vertexFilePath.c_str()))
		{
			Buffer vertexFileBuffer = Files::LoadFile(vertexFilePath.c_str());
			u8* vertexFile = vertexFileBuffer.As<u8>();

			// #TODO Find all vertex varyings and make sure they exist in the fragment shader

			std::vector<std::string> varyings;
			u64 vertIndex = 0;
			while (vertIndex < vertexFileBuffer.SizeInBytes())
			{
				const char c = *(char*)(vertexFile + vertIndex);
				if (c == 'v' && *(char*)(vertexFile + vertIndex + 1) == '_') // #TODO +1 could go out of range
				{
					u64 symbolLength = 0;
					std::string symbol;

					u64 tempIndex = vertIndex;
					char c2 = *(char*)(vertexFile + tempIndex);
					while (std::isalpha(c2) || std::isdigit(c2) || c2 == '_')
					{
						symbol += c2;
						++tempIndex; // #TODO Can go out of range
						c2 = *(char*)(vertexFile + tempIndex);
					}

					varyings.emplace_back(symbol);
					vertIndex += symbol.size();
				}
				++vertIndex;
			}

			std::string fragmentFilePath = Paths::Shader(Files::FileNameNoExt(a_FragmentFilePath).string().c_str());
			if (Files::Exists(fragmentFilePath.c_str()))
			{
				Buffer fragmentFileBuffer = Files::LoadFile(fragmentFilePath.c_str());
				u8* fragmentFile = fragmentFileBuffer.As<u8>();

				u64 fragIndex = 0;
				while (fragIndex < fragmentFileBuffer.SizeInBytes())
				{
					const char c = *(char*)(fragmentFile + fragIndex);
					if (c == 'v' && *(char*)(fragmentFile + fragIndex + 1) == '_') // #TODO +1 could go out of range
					{
						u64 symbolLength = 0;
						std::string symbol;

						u64 tempIndex = fragIndex;
						char c2 = *(char*)(fragmentFile + tempIndex);
						while (std::isalpha(c2) || std::isdigit(c2) || c2 == '_')
						{
							symbol += c2;
							++tempIndex; // #TODO Can go out of range
							c2 = *(char*)(fragmentFile + tempIndex);
						}

						for (size_t i = 0; i < varyings.size(); i++)
						{
							if (varyings[i] == symbol)
							{
								varyings.erase(varyings.begin() + i);
								break;
							}
						}

						fragIndex += symbol.size();
					}
					++fragIndex;
				}

				if (!varyings.empty())
				{
					ASSERT(false, "Varying mismatch! Possible Unused varying!");
				}
			}
		}
		else
		{
			ASSERT(a_VertexFilePath && a_VertexFilePath, "Invalid file paths!");
		}

		int bp = 0;
	}

}