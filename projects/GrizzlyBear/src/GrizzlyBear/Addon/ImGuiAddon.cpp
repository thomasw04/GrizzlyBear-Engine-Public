#include <gbpch.h>

#include <imgui/imgui.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui_internal.h>

#include "ImGuiAddon.h"

namespace GrizzlyBear {

	namespace Addon {

		void imgui_cheader(const char* label, imgui_state_func func, imgui_callback_func cfunc)
		{
			//Get cheader state
			std::pair<bool*, bool*> state = func();

			ImGui::PushID(label);

			ImVec2 startPos = ImGui::GetCursorScreenPos();
			ImVec2 endPos;

			endPos.x += startPos.x + ImGui::GetWindowContentRegionWidth() - 2;
			endPos.y += startPos.y + ImGui::CalcTextSize("T").y + 9;

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.57f, 0.99f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.47f, 0.47f, 0.47f, 1.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);

			if (ImGui::Button("##Button", ImVec2(ImGui::GetWindowContentRegionWidth() - 2, ImGui::CalcTextSize("T").y + 9)))
			{
				*state.first = !(*state.first);
			}

			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			//ImGui::GetWindowDrawList()->AddRect(startPos, endPos, ImGui::ColorConvertFloat4ToU32(ImVec4(0.37f, 0.37f, 0.37f, 1.0f)), 4.0f);
			ImGui::GetWindowDrawList()->AddRectFilled(startPos, endPos, ImGui::ColorConvertFloat4ToU32(ImVec4(0.33f, 0.35f, 0.36f, 0.53f)), 4.0f);

			ImGui::SetCursorScreenPos(ImVec2(startPos.x + 6, startPos.y + 1));

			ImGui::BeginChild("##Child", ImVec2(20, 20));

			imgui_checkbox("##Checkbox", state.second, ImVec2(0.0f, 2.0f));

			ImGui::EndChild();

			ImGui::SameLine();

			ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, startPos.y + 4));

			ImGui::Text(label);

			if (!(*state.second))
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}
			else
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, false);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha);
			}

			ImGui::PopID();

			if (*state.first)
			{
				ImGui::Indent();

				cfunc();

				ImGui::Unindent();

				ImGui::Separator();
			}

			ImGui::PopItemFlag();
			ImGui::PopStyleVar();
		}

		bool imgui_checkbox(const char* label, bool* value, const ImVec2& offset)
		{
			ImGuiWindow* window = ImGui::GetCurrentWindow();
			if (window->SkipItems)
				return false;

			ImGuiContext& g = *GImGui;
			ImGuiStyle& style = g.Style;

			//Save the previous style
			const ImGuiStyle oldStyle = style;

			style.FramePadding.y = 1.0f;

			const float square_sz = ImGui::GetFrameHeight();

			const ImGuiID id = window->GetID(label);
			const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

			ImVec2 pos = window->DC.CursorPos;

			pos += offset;

			const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
			ImGui::ItemSize(total_bb, style.FramePadding.y);
			if (!ImGui::ItemAdd(total_bb, id))
				return false;

			bool hovered, held;
			bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
			if (pressed)
			{
				*value = !(*value);
				ImGui::MarkItemEdited(id);
			}

			const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
			ImGui::RenderNavHighlight(total_bb, id);
			ImGui::RenderFrame(check_bb.Min, check_bb.Max, ImGui::GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, style.FrameRounding);
			ImU32 check_col = ImGui::GetColorU32(ImGuiCol_CheckMark);
			bool mixed_value = (g.LastItemData.InFlags & ImGuiItemFlags_MixedValue) != 0;
			if (mixed_value)
			{
				// Undocumented tristate/mixed/indeterminate checkbox (#2644)
				ImVec2 pad(ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)), ImMax(1.0f, IM_FLOOR(square_sz / 3.6f)));
				window->DrawList->AddRectFilled(check_bb.Min + pad, check_bb.Max - pad, check_col, style.FrameRounding);
			}
			else if (*value)
			{
				const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 6.0f));
				ImGui::RenderCheckMark(window->DrawList, check_bb.Min + ImVec2(pad, pad), check_col, square_sz - pad * 2.0f);
			}

			if (g.LogEnabled)
				ImGui::LogRenderedText(&total_bb.Min, *value ? "[x]" : "[ ]");
			if (label_size.x > 0.0f)
			{
				ImGui::RenderText(ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x + 3.0f, (check_bb.Min.y - 1.0f) + style.FramePadding.y), label);
			}

			IMGUI_TEST_ENGINE_ITEM_INFO(id, label, window->DC.ItemFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));

			//Assign the previous style
			style = oldStyle;

			return pressed;
		}

		bool imgui_grey_out(bool greyout, imgui_callback_func cfunc)
		{
			if (greyout)
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
			}
			else
			{
				ImGui::PushItemFlag(ImGuiItemFlags_Disabled, false);
				ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha);
			}

			cfunc();

			ImGui::PopItemFlag();
			ImGui::PopStyleVar();

			return true;
		}

		bool imgui_dropdown_button(const char* label, ImVec2 size, imgui_callback_func cfunc)
		{
			bool pressed = ImGui::Button(label, size);

			ImGuiWindow* window = ImGui::GetCurrentWindow();
			ImVec2 mousePos = ImGui::GetCursorPos();


			ImVec2 center = { window->Pos.x + mousePos.x + (size.x / 2), window->Pos.y + (mousePos.y / 2) };
			center.y -= 8.0f * 0.25f;
			ImVec2 a = center + ImVec2(0, 1) * 8.0f;
			ImVec2 b = center + ImVec2(-0.866f, -0.5f) * 8.0f;
			ImVec2 c = center + ImVec2(0.866f, -0.5f) * 8.0f;

			window->DrawList->AddTriangleFilled(a, b, c, ImGui::GetColorU32(ImGuiCol_Text));

			ImVec2 popupPos = { window->Pos.x + mousePos.x - size.x, window->Pos.y + mousePos.y + size.y };

			ImGui::SetNextWindowPos(popupPos);

			if (pressed)
			{
				ImGui::OpenPopup(label);
			}

			if (ImGui::BeginPopup(label))
			{
				const ImGuiStyle& style = GImGui->Style;
				ImGui::PushStyleColor(ImGuiCol_FrameBg, style.Colors[ImGuiCol_Button]);
				ImGui::PushStyleColor(ImGuiCol_WindowBg, style.Colors[ImGuiCol_Button]);
				ImGui::PushStyleColor(ImGuiCol_ChildBg, style.Colors[ImGuiCol_Button]);

				cfunc();

				ImGui::PopStyleColor(3);
				ImGui::EndPopup();
				return true;
			}

			return false;
		}


		void imgui_render_popups()
		{
			if (ImGui::BeginPopup("Duplicate"))
			{
				ImGui::Text("The requested data structure already exists.");

				ImGui::EndPopup();
			}

			if (ImGui::BeginPopup("Not found"))
			{
				ImGui::Text("The requested data structure does not exists.");

				ImGui::EndPopup();
			}

			if (ImGui::BeginPopup("Error"))
			{
				ImGui::Text("An error was triggered, please try again later.");

				ImGui::EndPopup();
			}

			if (ImGui::BeginPopup("Success"))
			{
				ImGui::Text("The requested action was performed successfully.");

				ImGui::EndPopup();
			}
		}
	}

}