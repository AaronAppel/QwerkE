#define IMGUI_DEFINE_MATH_OPERATORS
#include "utilities/builders.h"
#include "utilities/widgets.h"

#include "imgui_node_editor.h"
#include "blueprints-example-types.h"
#include "imgui_internal.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>

static inline ImRect ImGui_GetItemRect();
static inline ImRect ImRect_Expanded(const ImRect& rect, float x, float y);

namespace ed = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

using namespace ax;

using ax::Widgets::IconType;

static ed::EditorContext* m_Editor = nullptr;

static bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f);

ImTextureID LoadTexture(const char* path);
ImTextureID CreateTexture(const void* data, int width, int height);
void        DestroyTexture(ImTextureID texture);
int         GetTextureWidth(ImTextureID texture);
int         GetTextureHeight(ImTextureID texture);

struct Example
{
    int GetNextId();

    ed::LinkId GetNextLinkId();

    void TouchNode(ed::NodeId id);

    float GetTouchProgress(ed::NodeId id);

    void UpdateTouch();

    Node* FindNode(ed::NodeId id);

    Link* FindLink(ed::LinkId id);

    Pin* FindPin(ed::PinId id);

    bool IsPinLinked(ed::PinId id);

    bool CanCreateLink(Pin* a, Pin* b);

    void BuildNode(Node* node);

    Node* SpawnInputActionNode();

    Node* SpawnBranchNode();

    Node* SpawnDoNNode();

    Node* SpawnOutputActionNode();

    Node* SpawnPrintStringNode();

    Node* SpawnMessageNode();

    Node* SpawnSetTimerNode();

    Node* SpawnLessNode();

    Node* SpawnWeirdNode();

    Node* SpawnTraceByChannelNode();

    Node* SpawnTreeSequenceNode();

    Node* SpawnTreeTaskNode();

    Node* SpawnTreeTask2Node();

    Node* SpawnComment();

    Node* SpawnHoudiniTransformNode();

    Node* SpawnHoudiniGroupNode();

    void BuildNodes();

    void OnStart();

    void OnStop();

    ImColor GetIconColor(PinType type);

    void DrawPinIcon(const Pin& pin, bool connected, int alpha);

    void ShowStyleEditor(bool* show = nullptr);

    void ShowLeftPane(float paneWidth);

    void OnFrame(float deltaTime);

    int                  m_NextId = 1;
    const int            m_PinIconSize = 24;
    std::vector<Node>    m_Nodes;
    std::vector<Link>    m_Links;
    ImTextureID          m_HeaderBackground = nullptr;
    ImTextureID          m_SaveIcon = nullptr;
    ImTextureID          m_RestoreIcon = nullptr;
    const float          m_TouchTime = 1.0f;
    std::map<ed::NodeId, float, NodeIdLess> m_NodeTouchTime;
    bool                 m_ShowOrdinals = false;
};
