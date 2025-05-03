#include "QE_EditorWindowTimeline.h"

#include "Libraries/im-neo-sequencer/imgui_neo_sequencer.h"

namespace QwerkE {

	namespace Editor {

		// From: https://gitlab.com/GroGy/im-neo-sequencer
		int32_t currentFrame = 0;
		int32_t startFrame = 0;
		int32_t endFrame = 60;
		bool transformOpen = true;
		std::vector<ImGui::FrameIndexType> keys = { 0, 10, 24 };

		void EditorWindowTimeline::DrawInternal()
		{
            if (false) // #TODO Fix bx/imgui deallocation crash on close (similar to node editors)
            {
                ImGui::Text("#TODO: Finish implementing!");
                return;
            }

            const bool deleteKeyframe = ImGui::Button("Delete");
            if (ImGui::BeginNeoSequencer("Sequencer1", &currentFrame, &startFrame, &endFrame, { 0, 0 },
                ImGuiNeoSequencerFlags_EnableSelection |
                ImGuiNeoSequencerFlags_Selection_EnableDragging |
                ImGuiNeoSequencerFlags_Selection_EnableDeletion))
            {
                if (ImGui::BeginNeoGroup("Transform", &transformOpen))
                {
                    if (ImGui::BeginNeoTimelineEx("Position"))
                    {
                        for (auto&& v : keys)
                        {
                            ImGui::NeoKeyframe(&v);
                            // Per keyframe code here
                        }

                        if (deleteKeyframe)
                        {
                            uint32_t count = ImGui::GetNeoKeyframeSelectionSize();

                            ImGui::FrameIndexType* toRemove = new ImGui::FrameIndexType[count];

                            ImGui::GetNeoKeyframeSelection(toRemove);

                            // Delete keyframes from your structure
                        }
                        ImGui::EndNeoTimeLine();
                    }
                    ImGui::EndNeoGroup();
                }

                ImGui::EndNeoSequencer();
            }
		}

	}

}