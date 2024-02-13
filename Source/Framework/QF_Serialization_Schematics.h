
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

        void SaveMaterialSchematic(Material* material);

        void SaveShaderSchematic(ShaderProgram* shader);

    }

}