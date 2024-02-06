
namespace QwerkE
{

    class Material;
    class RenderComponent;
    class ShaderProgram;

    namespace Serialization
    {

        void SaveObjectSchematic(RenderComponent* rComp);
        RenderComponent* LoadRenderComponentFromSchematic(const char* schematicPath);

        void SaveMaterialSchematic(Material* material);
        void LoadMaterialSchematic(const char* schematicPath, Material* material);
        Material* LoadMaterialSchematic(const char* schematicPath);

        void SaveMaterialSchematic(Material* material);
        Material* LoadMaterialSchematic(const char* schematicPath);

        void SaveShaderSchematic(ShaderProgram* shader);
        ShaderProgram* LoadShaderSchematic(const char* schematicPath);

    }

}