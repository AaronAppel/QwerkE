#include "QF_Serialization_Schematics.h"

#include "Libraries/cJSON/QC_cJSON.h"

#include "QF_Defines.h"
#include "QF_FileUtilities.h"
#include "QF_Material.h"
#include "QF_RenderComponent.h"
#include "QF_ShaderComponent.h"
#include "QF_Serialization.h"

namespace QwerkE {

	namespace Serialization {

		void SaveObjectSchematic(RenderComponent* rComp)
		{
			const char* filePath = ObjectSchematicsFolderPath(StringAppend(rComp->GetSchematicName().c_str(), object_schematic_ext));

			if (!FileExists(filePath))
			{
				CreateEmptycJSONFile(filePath);
			}

			cJSON* root = OpencJSONStream(filePath);
			if (!root) { return; }

			AddItemToRoot(root, CreateString("Name", rComp->GetSchematicName().c_str()));

			cJSON* renderables = CreateArray("Renderables");

			// #TODO Better way to get access to renderable values?
			std::vector<Renderable>* renderablesList = (std::vector<Renderable>*)rComp->LookAtRenderableList();

			for (size_t i = 0; i < renderablesList->size(); i++)
			{
				// #TODO Set the renderable names
				cJSON* renderable = CreateArray(StringAppend("R", std::to_string(i).c_str()));

				AddItemToArray(renderable, CreateString("Shader", renderablesList->at(i).GetShader()->GetName().c_str()));
				AddItemToArray(renderable, CreateString("Material", renderablesList->at(i).GetMaterial()->GetMaterialName().c_str()));
				AddItemToArray(renderable, CreateString("MeshFile", renderablesList->at(i).GetMesh()->GetFileName().c_str()));
				AddItemToArray(renderable, CreateString("MeshName", renderablesList->at(i).GetMesh()->GetName().c_str()));

				AddItemToArray(renderables, renderable);
			}

			AddItemToRoot(root, renderables);

			PrintRootObjectToFile(filePath, root);
			ClosecJSONStream(root);
		}

		RenderComponent* LoadRenderComponentFromSchematic(const char* schematicPath)
		{
			RenderComponent* rComp = new RenderComponent();
			Serialization::DeserializeJsonFromFile(schematicPath, *rComp);
			rComp->Activate();
			return rComp;
		}

		void SaveMaterialSchematic(Material* mat)
		{
			const char* filePath = TexturesFolderPath(StringAppend(mat->GetMaterialName().c_str(), material_schematic_ext));

			if (!FileExists(filePath))
			{
				CreateEmptycJSONFile(filePath);
			}

			// save data
			// Note: I am not saving texture handles because they are not reliable.
			// #TODO Should I use struct member names like "s_FileName"?
			cJSON* root = OpencJSONStream(filePath);
			if (!root) { return; }

			AddItemToRoot(root, CreateString("m_Name", "MaterialSchematic1.msch"));

			//cJSON* OtherData = CreateArray("OtherData");
			//AddItemToArray(OtherData, CreateNumber("Shine", mat->s_Shine));
			// #TODO LightData AddItemToArray(OtherData, CreateNumber("LightData", mat->s_Shine)); {r,g,b,a}
			// AddItemToRoot(root, OtherData);

			cJSON* names = CreateArray("MaterialComponents");

			const std::map<eMaterialMaps, Texture*>* matComponents = mat->SeeMaterials();

			for (auto p : *matComponents)
			{
				Texture* texture = p.second;
				AddItemToArray(names, CreateString(std::to_string((int)p.first).c_str(), texture->s_FileName.c_str()));
			}

			// AddItemToArray(Names, CreateString("AmbientName", mat->s_AmbientName.c_str()));
			// AddItemToArray(Names, CreateString("DiffuseName", mat->s_DiffuseName.c_str()));
			// AddItemToArray(Names, CreateString("SpecularName", mat->s_SpecularName.c_str()));
			// AddItemToArray(Names, CreateString("EmissiveName", mat->s_EmissiveName.c_str()));
			// AddItemToArray(Names, CreateString("HeightName", mat->s_HeightName.c_str()));
			// AddItemToArray(Names, CreateString("NormalName", mat->s_NormalsName.c_str()));
			// AddItemToArray(Names, CreateString("ShininessName", mat->s_ShininessName.c_str()));
			// AddItemToArray(Names, CreateString("OpacityName", mat->s_OpacityName.c_str()));
			// AddItemToArray(Names, CreateString("DisplacementName", mat->s_DisplacementName.c_str()));
			// AddItemToArray(Names, CreateString("LightMapName", mat->s_LightMapName.c_str()));
			// AddItemToArray(Names, CreateString("ReflectionName", mat->s_ReflectionName.c_str()));

			AddItemToRoot(root, names);

			PrintRootObjectToFile(filePath, root);
			ClosecJSONStream(root);
		}

		void LoadMaterialSchematic(const char* schematicPath, Material* material)
		{
			Serialization::DeserializeJsonFromFile(schematicPath, *material);
			return;

			cJSON* root = OpencJSONStream(schematicPath);
			if (root)
			{
				// load mat data from file
				material->SetMaterialName(GetItemFromRootByKey(root, "m_Name")->valuestring);

				// load "other" data
				//cJSON* otherData = GetItemFromRootByKey(root, "OtherData");
				//mat->s_Shine = GetItemFromArrayByKey(otherData, "Shine")->valuedouble;
				// #TODO mat->s_LightValue = GetItemFromArrayByKey(otherData, "LightData")->valuedouble;

				// set texture names
				cJSON* textureNames = GetItemFromRootByKey(root, "TextureNames");

				for (unsigned int i = 0; i < GetArraySize(textureNames); i++)
				{
					cJSON* value = GetItemFromArrayByIndex(textureNames, i);
					if (value)
					{
						// ask for texture from resource manager, making sure the texture gets created
						// then add it to the material in its proper map "slot"
						material->AddTexture(Resources::GetTexture(value->valuestring), (eMaterialMaps)(value->string[0] - 48));
					}
				}
				// mat->s_AmbientName = GetItemFromArrayByKey(textureNames, "AmbientName")->valuestring;
				// mat->s_DiffuseName = GetItemFromArrayByKey(textureNames, "DiffuseName")->valuestring;
				// mat->s_SpecularName = GetItemFromArrayByKey(textureNames, "SpecularName")->valuestring;
				// mat->s_EmissiveName = GetItemFromArrayByKey(textureNames, "EmissiveName")->valuestring;
				// mat->s_HeightName = GetItemFromArrayByKey(textureNames, "HeightName")->valuestring;
				// mat->s_NormalsName = GetItemFromArrayByKey(textureNames, "NormalName")->valuestring;
				// mat->s_ShininessName = GetItemFromArrayByKey(textureNames, "ShininessName")->valuestring;
				// mat->s_OpacityName = GetItemFromArrayByKey(textureNames, "OpacityName")->valuestring;
				// mat->s_DisplacementName = GetItemFromArrayByKey(textureNames, "DisplacementName")->valuestring;
				// mat->s_LightMapName = GetItemFromArrayByKey(textureNames, "LightMapName")->valuestring;
				// mat->s_ReflectionName = GetItemFromArrayByKey(textureNames, "ReflectionName")->valuestring;
			}

			ClosecJSONStream(root);

			// load texture handles from Resources

			// #TODO Stop trying to load "Empty" files
			// material->s_AmbientHandle = Resources::GetTexture(mat->s_AmbientName.c_str());
			// material->s_DiffuseHandle = Resources::GetTexture(mat->s_DiffuseName.c_str());
			// material->s_SpecularHandle = Resources::GetTexture(mat->s_SpecularName.c_str());
			// material->s_EmissiveHandle = Resources::GetTexture(mat->s_EmissiveName.c_str());
			// material->s_HeightHandle = Resources::GetTexture(mat->s_HeightName.c_str());
			// material->s_NormalsHandle = Resources::GetTexture(mat->s_NormalsName.c_str());
			// material->s_ShininessHandle = Resources::GetTexture(mat->s_ShininessName.c_str());
			// material->s_OpacityHandle = Resources::GetTexture(mat->s_OpacityName.c_str());
			// material->s_DisplacementHandle = Resources::GetTexture(mat->s_DisplacementName.c_str());
			// material->s_LightMapHandle = Resources::GetTexture(mat->s_LightMapName.c_str());
			// material->s_ReflectionHandle = Resources::GetTexture(mat->s_ReflectionName.c_str());
		}

		void SaveShaderSchematic(ShaderProgram* shader)
		{
			const char* filePath = StringAppend(AssetsDir, "Shaders/LitMaterial", shader_schematic_ext);

			// if file does not exist, create one,otherwise overwrite data
			if (!FileExists(filePath))
			{
				CreateEmptycJSONFile(filePath);
			}

			cJSON* root = OpencJSONStream(filePath);
			if (!root) { return; }

			AddItemToRoot(root, CreateString("Name", shader->GetName().c_str()));

			AddItemToRoot(root, CreateString("vert", shader->GetVertShader()->GetName().c_str()));
			AddItemToRoot(root, CreateString("frag", shader->GetFragShader()->GetName().c_str()));
			AddItemToRoot(root, CreateString("geo", shader->GetGeoShader()->GetName().c_str()));

			PrintRootObjectToFile(filePath, root);
			ClosecJSONStream(root);
		}

		ShaderProgram* LoadShaderSchematic(const char* schematicPath)
		{
			ShaderProgram* shader = new ShaderProgram();

			Serialization::DeserializeJsonFromFile(schematicPath, *shader);
			return shader;
		}

	}

}