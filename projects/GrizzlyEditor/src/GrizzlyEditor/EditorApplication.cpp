#include "EditorApplication.h"

#include <GrizzlyBear/Core/BaseDefines.h>

#include "Modes/EditorDefaultMode.h"


namespace GrizzlyEditor {


	GrizzlyBear::ModuleState EditorApplication::on_module_init()
	{
		m_Window = GrizzlyBear::Ref<GrizzlyBear::Window>(GrizzlyBear::Window::create("GrizzlyBear Editor", 1280, 720));

		m_EditorModes.push_back(std::make_shared<EditorDefaultMode>(m_Window));
		return GrizzlyBear::ModuleState::GOOD;
	}

	bool EditorApplication::on_startup()
	{
		set_editor_mode(EditorType::STABLE_DEFAULT);
		return true;
	}

	void EditorApplication::on_update(GrizzlyBear::Timestep ts)
	{
		m_CurrentMode->on_update(ts);
	}

	void EditorApplication::on_gui_update(GrizzlyBear::Timestep ts)
	{
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Grizzly Dockspace", (bool*)0, window_flags);
		ImGui::PopStyleVar();

		ImGui::PopStyleVar(2);

		ImGuiID dockspace_id = ImGui::GetID("Grizzly Dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Mode"))
			{
				EditorType type = m_CurrentMode->get_type();

				if (ImGui::MenuItem("Stable: Default")			&& type != EditorType::STABLE_DEFAULT)			{ set_editor_mode(EditorType::STABLE_DEFAULT); }

				ImGui::Separator();

				if (ImGui::MenuItem("Exit")) GrizzlyBear::Core::shutdown();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Screen"))
			{
				if (ImGui::MenuItem("Windowed"))	{ m_Window->set_window_mode(GrizzlyBear::WindowMode::WINDOWED); }
				if (ImGui::MenuItem("Borderless"))	{ m_Window->set_window_mode(GrizzlyBear::WindowMode::BORDERLESS); }
				if (ImGui::MenuItem("Fullscreen"))	{ m_Window->set_window_mode(GrizzlyBear::WindowMode::FULLSCREEN); }

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Windows"))
			{
				for (auto it = m_CurrentMode->m_Windows.begin(); it != m_CurrentMode->m_Windows.end(); it++)
				{
					if (ImGui::MenuItem(it->first.c_str(), "", it->second->get_window_open())) { it->second->get_window_open() = !it->second->get_window_open(); }
				}

				ImGui::EndMenu();
			}

			m_CurrentMode->on_dockspace_menu_bar();

			ImGui::EndMenuBar();
		}

		ImGui::End();

		m_CurrentMode->on_gui_update(dockspace_id);
	}

	bool EditorApplication::on_shutdown()
	{
		m_CurrentMode->on_deactivate();
		return true;
	}

	GrizzlyBear::Ref<GrizzlyBear::Window> EditorApplication::get_main_window()
	{
		return m_Window;
	}

	void EditorApplication::set_editor_mode(EditorType type)
	{
		for (std::shared_ptr<EditorMode> mode : m_EditorModes)
		{
			if (mode->get_type() == type)
			{
				if(m_CurrentMode != nullptr) m_CurrentMode->on_deactivate();

				m_CurrentMode = mode;

				m_CurrentMode->on_activate();
				return;
			}
		}

		GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Error, "Editor mode is not available");
	}
}

int main(int argc, char* argv[])
{
	GrizzlyBear::Ref<GrizzlyEditor::EditorApplication> app = GrizzlyBear::CreateRef<GrizzlyEditor::EditorApplication>();

	if (argc < 2)
	{
		char pathToProject[255];

		std::cout << "Please enter the Project File: ";
		std::cin.getline(pathToProject, sizeof(pathToProject));

		GrizzlyBear::Core::run(pathToProject, app);
		return 0;
	}
	else
	{
		GrizzlyBear::Core::run(std::string(argv[1]), app);
	}

	std::cin.get();

	return EXIT_SUCCESS;
}





