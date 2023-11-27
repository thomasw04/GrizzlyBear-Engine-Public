#include "EditorApplication.h"

#include "Modes/EditorDefaultMode.h"
#include "Modes/EXPEditorMode3D.h"
#include "Modes/EditorPolygonMode.h"
#include "Modes/GameMode2D.h"

#include <vector>

#include <GrizzlyBear/Core/BaseDefines.h>
#include <GrizzlyBear/Core/Reflection/Reflector.h>

#include <GrizzlyBear/Script/ScriptManager.h>

#include "GrizzlyBear/Template/PerspectiveCamera.h"
#include "GrizzlyBear/Environment/Environment.h"

#include "GrizzlyBear/Core/Platform/Other/Ansicodes.h"

#include <boost/program_options.hpp>

#include <thread>
#include <chrono>

namespace GrizzlyEditor {


	GrizzlyBear::ModuleInfo EditorApplication::on_module_init()
	{
		m_Window = GrizzlyBear::Ref<GrizzlyBear::Window>(GrizzlyBear::Window::create("GrizzlyBear Editor", 1280, 720));

		m_EditorModes.push_back(std::make_shared<EditorDefaultMode>(m_Window));
		m_EditorModes.push_back(std::make_shared<EXPEditorMode3D>());
		m_EditorModes.push_back(std::make_shared<EditorPolgonMode>());
		//m_EditorModes.push_back(std::make_shared<GameMode2D>());
		return GrizzlyBear::ModuleInfo({ GrizzlyBear::ModuleState::GOOD, ""});
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
				if (ImGui::MenuItem("Stable: Play")				&& type != EditorType::STABLE_PLAY)				{ set_editor_mode(EditorType::STABLE_PLAY); }
				if (ImGui::MenuItem("Experimental: Default")    && type != EditorType::EXPERIMENTAL_DEFAULT)	{ set_editor_mode(EditorType::EXPERIMENTAL_DEFAULT); }
				if (ImGui::MenuItem("Experimental: Play")		&& type != EditorType::EXPERIMENTAL_PLAY)		{ set_editor_mode(EditorType::EXPERIMENTAL_PLAY); }
				if (ImGui::MenuItem("Experimental: 3D VWA")		&& type != EditorType::EXPERIMENTAL_3D_VWA)		{ set_editor_mode(EditorType::EXPERIMENTAL_3D_VWA); }
				if (ImGui::MenuItem("Experimental: Polygons")   && type != EditorType::EXPERIMENTAL_POLYGON)    { set_editor_mode(EditorType::EXPERIMENTAL_POLYGON); }

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

/*class Nested1
{
public:
	GRIZZLY_REFLECT_CLASS(Nested1)

public:
	Nested1(bool f) : f(f) {}

public:
	GRIZZLY_REFLECT_VARS
	(
		(bool) f
	)
};

class Nested
{
public:
	GRIZZLY_REFLECT_CLASS(Nested)

public:
	Nested(bool lol, int j)
		: lol(lol), j(j), n(true) {}

public:
	GRIZZLY_REFLECT_VARS
	(
		(bool) lol,
		(int) j,
		(Nested1) n
	)
};

GRIZZLY_SERIALIZE_CON_CLASS(Nested)

enum MYEnum
{
	First, Second
};

class Test
{
public:
	Test(int* test, bool test1)
		: enum1(MYEnum::Second), svec{Nested(true, 1), Nested(true, 2)}, test(test), test1(test1), nested(Nested(true, 9)), vec3(2.0f, 3.0f, 4.0f) {}

public:
	GRIZZLY_REFLECT_VARS
	(
		(MYEnum) enum1,
		(std::array<Nested, 2>) svec,
		(int*) test,
		(bool) test1,
		(Nested) nested,
		(glm::vec3) vec3
	)
};*/

/*#include <GrizzlyBear/Core/Thread.h>

struct t
{
	int a = 0;
	int arrayC[1000000];
	int b = 0;
};

t test;

GrizzlyBear::ThreadState<int> process_other_data(GrizzlyBear::ThreadActions actions)
{
	for (int i = 0; i < 100000000; i++)
	{
		test.b += 2;

		if (test.b % 3 == 0) test.b--;
	}

	co_return 0;
}

GrizzlyBear::ThreadState<int> process_data(GrizzlyBear::ThreadActions actions)
{
	GrizzlyBear::RunHNDL handle = actions.spawn_child(process_other_data);

	for (int i = 0; i < 100000000; i++)
	{
		test.a += 3;

		if (test.a % 3 == 0) test.a--;
	}

	co_yield handle;



	co_return 0;
}

void complete_unique_name()
{
	for (int i = 0; i < 100000000; i++)
	{
		test.a += 3;

		if (test.a % 3 == 0) test.a--;
	}
}

void d()
{
	for (int i = 0; i < 100000000; i++)
	{
		test.b += 2;

		if (test.b % 3 == 0) test.b--;
	}
}*/

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
	/*{
		GrizzlyBear::Timer t("Thread timer");

		for (int i = 0; i < 10; i++)
		{
			test.a = 0;
			test.b = 0;

			GrizzlyBear::RunHNDL handle = GrizzlyBear::ThreadManager::queue_thread(process_data, true);

			//complete_unique_name();
			//d();

			if(i != 9) t.round();

			//std::cout << "A: " << a << " B: " << b << std::endl;
		}

		std::cout << "A: " << test.a << " B: " << test.b << std::endl;
	}*/

	/*GrizzlyBear::Log::init();
	GrizzlyBear::Platform::init();

	GrizzlyBear::Ref<GrizzlyBear::Window> window = GrizzlyBear::Window::create("Test", 1200, 800);

	GrizzlyBear::Renderer::init(window->get_context(), GrizzlyBear::RendererHint_NoEnvironment, GrizzlyBear::AssetManagerHint_NoEnvironment);

	bool running = true;

	window->on_key(GrizzlyBear::KeyEvent::PRESSED, [&](const GrizzlyBear::KeyCode& e) -> bool
	{
		if (e.KEY_CODE == GRIZZLY_KEY_ESCAPE)
		{
			running = false;
			return true;
		}

		return false;
	});

	GrizzlyBear::Template::PerspectiveCamera camera(90, 16.0f/9.0f, false);

	GrizzlyBear::Timestep timestep;

	while (running)
	{
		timestep.update(GrizzlyBear::Platform::get_time());

		camera.on_update(window, timestep);

		GrizzlyBear::Renderer::begin_frame();
		GrizzlyBear::Renderer2D::begin_scene(camera.get_projection(), camera.calc_view());
		GrizzlyBear::Renderer2D::draw_quad({ 0.0f, 0.0f, 0.0f }, 0.0f, { 10.0f, 10.0f }, GrizzlyBear::MaterialProps2D());
		GrizzlyBear::Renderer2D::end_scene();
		GrizzlyBear::Renderer::end_frame();

		window->on_update();
	}*/

	/*HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleMode(console, ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT | DISABLE_NEWLINE_AUTO_RETURN);

	std::thread t([]()
	{
		std::string s;
		std::getline(std::cin, s);
	});

	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	CloseHandle(GetStdHandle(STD_INPUT_HANDLE));

	t.join();*/

	//auto n = po::split_unix(s);

	/*GrizzlyBear::AnsiTerminal an(std::cout);
	an.reset_graphics();

	std::cout << "[          ]";

	an.move_to(0, 2);

	for (int i = 0; i < 10; i++)
	{
		an.set_foreground(42);

		std::cout << "#";
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	an.move_down_beg(1);

	an.reset_graphics();*/

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
	
	/*nlohmann::json json;

	int i = 124;

	Test t(&i, true);

	GrizzlyBear::Reflector::serialize(t, json);

	std::cout << json << std::endl;

	std::string parse = json.dump();

	std::cout << "-----------------" << std::endl;

	nlohmann::json deser = nlohmann::json::parse(parse);

	Test t1;
	
	GrizzlyBear::Reflector::deserialize<Test>(t1, deser);

	std::cout << t1.enum1 << std::endl;

	std::cout << t1.svec[0].j << ", " << t1.svec[1].j << std::endl;

	std::cout << *(t1.test) << std::endl;
	std::cout << t1.test1 << std::endl;
	std::cout << t1.vec3.x << "," << t1.vec3.y << "," << t1.vec3.z << std::endl;

	std::cout << t1.nested.j << std::endl;
	std::cout << t1.nested.lol << std::endl;

	std::cout << t1.nested.n.f << std::endl;*/

	return 0;
}





