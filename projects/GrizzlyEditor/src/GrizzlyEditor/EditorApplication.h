#pragma once

#include <GrizzlyBear.h>

#include "EditorWindow/WindowManager.h"

#include "Utilities/EditorType.h"

namespace GrizzlyEditor {

	class EditorMode
	{
		friend class EditorApplication;

	public:

		virtual ~EditorMode() = default;

		virtual void on_activate() = 0;

		virtual void on_update(GrizzlyBear::Timestep ts) = 0;

		virtual void on_gui_update(unsigned int dockspaceID) = 0;

		virtual void on_dockspace_menu_bar() = 0;

		virtual void on_deactivate() = 0;

		virtual EditorType get_type() = 0;

	protected:
		std::unordered_map<std::string, std::shared_ptr<GrizzlyEditor::EditorWindow>> m_Windows;

		template<typename T>
		inline std::shared_ptr<T> get_editor_window(const std::string& name)
		{
			return std::dynamic_pointer_cast<T>(m_Windows.at(name));
		}

		template<typename T>
		inline std::shared_ptr<T> insert_and_return(const std::string& name, std::shared_ptr<T> window)
		{
			return std::dynamic_pointer_cast<T>(std::get<0>(m_Windows.insert({ name, window }))->second);
		}
	};

	class EditorApplication : public GrizzlyBear::GrizzlyApplication
	{
	public:

		virtual ~EditorApplication() = default;

		virtual GrizzlyBear::ModuleState on_module_init() override;

		virtual bool on_startup() override;

		virtual void on_update(GrizzlyBear::Timestep ts) override;
		virtual void on_gui_update(GrizzlyBear::Timestep ts) override;

		virtual bool on_shutdown() override;

		virtual GrizzlyBear::Ref<GrizzlyBear::Window> get_main_window() override;

	private:
		void set_editor_mode(EditorType type);

		std::shared_ptr<GrizzlyEditor::EditorWindow> get_editor_window(const std::string& name);

	private:
		std::shared_ptr<EditorMode> m_CurrentMode = nullptr;

		std::vector<std::shared_ptr<EditorMode>> m_EditorModes;
		GrizzlyBear::Ref<GrizzlyBear::Window> m_Window;
	};

}
