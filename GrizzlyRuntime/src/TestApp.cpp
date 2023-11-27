#include <GrizzlyBear.h>

#include <glm/gtc/type_ptr.hpp>

class Player : public GrizzlyBear::Entity2D
{
public:
	Player() {}

	virtual void on_spawn() override
	{
		m_Transform.POSITION = { 1.0f, 1.0f, 0.0f };
		m_Transform.SCALE = { 1.0f, 1.0f };

		make_visible(true);
		enable_events(true);
	}

	virtual void on_update(GrizzlyBear::Timestep ts) override
	{
		GrizzlyBear::MaterialProps2D& props = get_material();

		if (!is_animating())
		{
			props.TINT = { 0.5f, 1.0f, 0.3f, 1.0f };
		}

		if (is_key_pressed(GRIZZLY_KEY_D))
		{
			m_Transform.POSITION.x += 1.0f * ts;
		}
		else if(is_key_pressed(GRIZZLY_KEY_A))
		{
			m_Transform.POSITION.x -= 1.0f * ts;
		}
	}

	virtual bool on_key(GrizzlyBear::KeyEvent ev, GrizzlyBear::KeyCode code)
	{
		if (ev == GrizzlyBear::KeyEvent::PRESSED)
		{
			if (is_key_pressed(GRIZZLY_KEY_LEFT_SHIFT) && code.KEY_CODE == GRIZZLY_KEY_P)
			{
				play_animation("anims/TestAnim.ganim");
			}
		}

		return false;
	}

	virtual bool on_mouse(GrizzlyBear::MouseEvent ev, GrizzlyBear::MouseCode code)
	{
		return false;
	}


	virtual void on_delete() override
	{

	}
};


class TestApp : public GrizzlyBear::GrizzlyApplication
{
public:

	virtual GrizzlyBear::ModuleInfo on_module_init() override
	{
		//Create the game window
		m_Window = GrizzlyBear::Ref<GrizzlyBear::Window>(GrizzlyBear::Window::create("TestApp", 1280, 720));

		return { GrizzlyBear::ModuleState::GOOD, ""};
	}

	virtual bool on_startup() override
	{
		//Register a bunch of callbacks 
		//(Event System ist implemented like a Stack -> every on_<event> call is a push on the stack and needs to be removed afterwards -> Use the Overlay template for RAII)
		//Return true, blocks the event to be propagated further in the stack. Return false not.
		m_Window->on_key(GrizzlyBear::KeyEvent::PRESSED, [&](const GrizzlyBear::KeyCode& e) -> bool
		{
			if (e.KEY_CODE == GRIZZLY_KEY_F11)
			{
				m_Window->set_window_mode(GrizzlyBear::WindowMode::TOGGLE);
			}

			if (e.KEY_CODE == GRIZZLY_KEY_V && m_Window->is_key_pressed(GRIZZLY_KEY_LEFT_SHIFT))
			{
				m_Window->set_v_sync(GrizzlyBear::VsyncMode::TOGGLE);
				GRIZZLY_CLIENT_LOG(GrizzlyBear::LogLevel::Warn, "Toggled Vsync to: {0}", (int)m_Window->get_v_sync());
			}

			return false;
		});

		m_DefaultBuffer = m_Window->get_context()->create_frame_buffer(m_Window->get_width(), m_Window->get_height());

		//Init needed systems
		m_MasterRenderer = GrizzlyBear::CreateRef<GrizzlyBear::Renderer>(get_main_window());
		m_GuiRenderer = std::make_shared<GrizzlyBear::Renderer2D>(m_Window->get_context(), GrizzlyBear::RendererHint_None);

		m_SceneManager = std::make_shared<GrizzlyBear::Template::SceneManager>(m_Window);
		m_Camera = std::make_shared<GrizzlyBear::Template::OrthographicCamera>(m_Window);
		m_GuiCamera = std::make_shared<GrizzlyBear::Template::GuiCamera>(m_Window);

		//Register our camera as the viewport for the scene.
		//The camera will now be managed und updated by the scene (no need to register callbacks etc.)
		m_SceneManager->set_custom_camera(m_Camera);

		auto scene = m_SceneManager->load_scene("TestScene");

		if (scene == nullptr) return false;

		GrizzlyBear::CompSprite redQuad;

		redQuad.MATERIAL.TEXTURE_FILEPATH = "assets/textures/Chessboard.png";
		redQuad.MATERIAL.TINT = { 1.0f, 1.0f, 1.0f, 1.0f };

		GrizzlyBear::CompTransformation quadTrans;

		quadTrans.m_Size = { 1.0f, 1.0f, 1.0f};

		GrizzlyBear::CompScript playerScript;

		playerScript.NATIVE = false;
		playerScript.FILE_PATH = "assets/scripts/ExampleEntity.lua";

		//Load our scene, this will create the scene (in this case not from a file) and load all the needed assets.
		//Stop scene creation if it already exists
		//Now we are creating a template component, which is basically a red quad.
		//And a template transform component, to set the size of the quad to non zero (so that we can see it)
		//And a template script component, to interact with the quad

		//Finally we register the components in the scene
		scene->set_template_component<GrizzlyBear::CompSprite>("RedQuad", redQuad);
		scene->set_template_component<GrizzlyBear::CompTransformation>("QuadTrans", quadTrans);
		scene->set_template_component<GrizzlyBear::CompScript>("PlayerScript", playerScript);

		//After we registered the components we need to create a template entity from the components we registered.
		//We will use the dynamic version for both components so that we are able to change the components at runtime, if we wish to do so (e.g. reposition the quads)
		scene->set_template_entity<GrizzlyBear::CompSprite, GrizzlyBear::CompTransformation, GrizzlyBear::CompScript>("Player",
		{ 
			{ GrizzlyBear::MemoryType::Dynamic, "RedQuad"     },
			{ GrizzlyBear::MemoryType::Dynamic, "QuadTrans"   },
			{ GrizzlyBear::MemoryType::Dynamic, "PlayerScript"}
		});

		//Now we are adding a bunch of entities to use our templates
		GrizzlyBear::EntityHandle testEntity1 = scene->add_entity();
		GrizzlyBear::EntityHandle testEntity2 = scene->add_entity();
		GrizzlyBear::EntityHandle testEntity3 = scene->add_entity();
		GrizzlyBear::EntityHandle testEntity4 = scene->add_entity();

		// And finally we are instantiating our template entity five times.Now our entities are visible by the next frame.
		scene->set_entity(testEntity1, "Player");
		scene->set_entity(testEntity2, "Player");
		scene->set_entity(testEntity3, "Player");
		scene->set_entity(testEntity4, "Player");

		//But we also need to reposition the quads a little bit, otherwise they will all be above each other
		scene->get_component<GrizzlyBear::CompTransformation>(GrizzlyBear::MemoryType::Dynamic, testEntity1)->m_Position = { 0.0f, 0.0f, 0.0f };
		scene->get_component<GrizzlyBear::CompTransformation>(GrizzlyBear::MemoryType::Dynamic, testEntity2)->m_Position = { 2.0f, 0.0f, 0.0f };
		scene->get_component<GrizzlyBear::CompTransformation>(GrizzlyBear::MemoryType::Dynamic, testEntity3)->m_Position = { 4.0f, 0.0f, 0.0f };
		scene->get_component<GrizzlyBear::CompTransformation>(GrizzlyBear::MemoryType::Dynamic, testEntity4)->m_Position = { 6.0f, 0.0f, 0.0f };


		GrizzlyBear::EntityHandle player = scene->add_entity();
		GrizzlyBear::Ref<Player> pScript = std::make_shared<Player>();
		scene->set_native_script(player, pScript);

		//And start the runtime of the scripts
		scene->start_runtime();

		//Not to forget that the scene must be set in the scene manager in order to render it
		m_SceneManager->set_scene("TestScene");
		return true;
	}

	virtual void on_update(GrizzlyBear::Timestep ts) override
	{
		m_MasterRenderer->begin_frame(m_SceneManager->get_scene()->get_background_color());

		m_GuiRenderer->begin_scene({ m_GuiCamera->get_projection(), m_GuiCamera->calc_view() });

		m_GuiRenderer->draw_line({
			{-6.0f, -4.0f, 0.0f},
			{-6.0f, 4.0f, 0.0f},
			{6.0f, 4.0f, 0.0f},
			{6.0f, -4.0f, 0.0f},
			{-6.0f, -4.0f, 0.0f} },
			{0.0f, 1.0f, 1.0f, 1.0f},1.0f);

		m_GuiRenderer->end_scene();

		m_SceneManager->on_update(ts);

		m_MasterRenderer->end_frame();
	}

	virtual void on_gui_update(GrizzlyBear::Timestep ts) override
	{
		m_Graph.update(ts.get_millis());

		ImGui::Begin("Frame");
		ImGui::PlotLines("##Frametime", m_Graph.get_graph(), 100, m_Graph.get_xpos(), "Frametime in ms", 0.0f, 100.0f, ImVec2(0, 100));
		ImGui::Text("Frametime in ms: %.2f", ts.get_millis());
		ImGui::End();

		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Background", glm::value_ptr(m_BackgroundColorEdit));
		m_SceneManager->get_scene()->set_background_color(m_BackgroundColorEdit);
		if (ImGui::Button("Click Me!"))
		{
			GRIZZLY_CORE_LOG(GrizzlyBear::LogLevel::Trace, "LOOOL");
		}
		ImGui::End();
	}

	virtual bool on_shutdown() override
	{
		m_SceneManager->get_scene()->stop_runtime();

		m_Window->pop_key(GrizzlyBear::KeyEvent::PRESSED);
		return true;
	}

	virtual GrizzlyBear::Ref<GrizzlyBear::Window> get_main_window() override
	{
		return m_Window;
	}

private:
	GrizzlyBear::Ref<GrizzlyBear::Window> m_Window;

	GrizzlyBear::Ref<GrizzlyBear::FrameBuffer> m_DefaultBuffer;
	GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> m_SceneManager;
	GrizzlyBear::Ref<GrizzlyBear::Template::OrthographicCamera> m_Camera;
	GrizzlyBear::Ref<GrizzlyBear::Template::GuiCamera> m_GuiCamera;

	GrizzlyBear::Ref<GrizzlyBear::Renderer> m_MasterRenderer;
	GrizzlyBear::Ref<GrizzlyBear::Renderer2D> m_GuiRenderer;

	GrizzlyBear::Template::SimpleGraph m_Graph;

	glm::vec3 m_BackgroundColorEdit;
};

int main(int argc, char* argv[])
{
	//Create the app
	GrizzlyBear::Ref<TestApp> app = std::make_shared<TestApp>();

	//Run the app
	GrizzlyBear::Core::run(std::string(argv[1]), app);
}