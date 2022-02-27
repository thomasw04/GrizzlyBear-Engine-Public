#include "ObjectEditorWindow.h"

#include <glm/gtc/type_ptr.hpp>

#include <GrizzlyBear.h>

namespace GrizzlyEditor {

	ObjectEditorWindow::ObjectEditorWindow(bool open, GrizzlyBear::Ref<GrizzlyBear::Window> window, GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> sceneManager)
		: EditorWindow(open, window)
	{
		m_SceneManager = sceneManager;
	}

	ObjectEditorWindow::ObjectEditorWindow(bool open, const ImVec2& position, const ImVec2& size, GrizzlyBear::Ref<GrizzlyBear::Window> window, GrizzlyBear::Ref<GrizzlyBear::Template::SceneManager> sceneManager)
		: EditorWindow(open, position, size, window)
	{
		m_SceneManager = sceneManager;
	}

	ObjectEditorWindow::~ObjectEditorWindow()
	{
	}

	void ObjectEditorWindow::set_current_entity(GrizzlyBear::EntityHandle entityHandle)
	{
		GrizzlyBear::Ref<GrizzlyBear::Scene> scene = m_SceneManager->get_scene();

		if (scene->has_component<GrizzlyBear::CompMesh2D>(GrizzlyBear::MemoryType::Dynamic, m_EntityHandle))
			scene->get_component<GrizzlyBear::CompMesh2D>(GrizzlyBear::MemoryType::Dynamic, m_EntityHandle)->m_Material.HIGHLIGHT = false;

		if (entityHandle == GrizzlyBear::EntityHandle::none())
		{
			m_EntityHandle = entityHandle;
			return;
		}

		if (scene->has_component<GrizzlyBear::CompMesh2D>(GrizzlyBear::MemoryType::Dynamic, entityHandle))
			scene->get_component<GrizzlyBear::CompMesh2D>(GrizzlyBear::MemoryType::Dynamic, entityHandle)->m_Material.HIGHLIGHT = true;

		m_EntityHandle = entityHandle;

		m_ComponentMesh2DEnabled = scene->has_component<GrizzlyBear::CompMesh2D>(GrizzlyBear::MemoryType::Dynamic, m_EntityHandle);
		m_ComponentCameraEnabled = scene->has_component<GrizzlyBear::CompCamera>(GrizzlyBear::MemoryType::Dynamic, m_EntityHandle);
		m_ComponentWidgetEnabled = scene->has_component<GrizzlyBear::CompWidget>(GrizzlyBear::MemoryType::Dynamic, m_EntityHandle);
		m_ComponentScriptEnabled = scene->has_component<GrizzlyBear::CompScript>(GrizzlyBear::MemoryType::Dynamic, m_EntityHandle);

		GrizzlyBear::CompMesh2D* mesh2D = scene->get_component<GrizzlyBear::CompMesh2D>(GrizzlyBear::MemoryType::Dynamic, m_EntityHandle);
		GrizzlyBear::CompWidget* widget = scene->get_component<GrizzlyBear::CompWidget>(GrizzlyBear::MemoryType::Dynamic, m_EntityHandle);
		GrizzlyBear::CompScript* script = scene->get_component<GrizzlyBear::CompScript>(GrizzlyBear::MemoryType::Dynamic, m_EntityHandle);

		m_Mesh2DTextureFilePath = mesh2D->m_Material.TEXTURE_FILEPATH;
		m_WidgetTextureFilePath = widget->m_Texture;
		m_ScriptFilePath = script->FILE_PATH;
		
		if (scene->get_lookup_table().exist_id(m_EntityHandle.get_id()))
		{
			m_EntityName = scene->get_lookup_table().get_name(m_EntityHandle.get_id());
		}
		else
		{
			m_EntityName = std::to_string(m_EntityHandle.get_id());

			scene->get_lookup_table().create_lookup(m_EntityName, m_EntityHandle.get_id());
		}
	}

	void ObjectEditorWindow::render_window()
	{
		if (m_EntityHandle == GrizzlyBear::EntityHandle::none()) return;

		GrizzlyBear::Ref<GrizzlyBear::Scene> scene = m_SceneManager->get_scene();

		GrizzlyBear::CompTransformation2D* compTransform = scene->get_component<GrizzlyBear::CompTransformation2D>(GrizzlyBear::MemoryType::Dynamic, m_EntityHandle);

		ImGui::Text("%s", m_EntityName.c_str());
		ImGui::Separator();

		if(ImGui::BeginTabBar("##entity_header", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Description"))
			{
				ImGui::Text("ID: %i", m_EntityHandle.get_id());

					if (m_EntityHandle.get_type() == GrizzlyBear::MemoryType::Dynamic)
					{
						ImGui::Text("Type: Dynamic");
					}
					else if (m_EntityHandle.get_type() == GrizzlyBear::MemoryType::Static)
					{
						ImGui::Text("Type: Static");
					}
					else
					{
						ImGui::Text("Type: Invalid");
					}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Components"))
			{
				if (ImGui::CollapsingHeader("Component: Transform2D"))
				{
					ImGui::Separator();

					ImGui::DragFloat3("Position", glm::value_ptr(compTransform->m_Position), 0.01f);
					ImGui::DragFloat("Rotation", &compTransform->m_Rotation, 0.1f, 0.0f, 360.0f);
					ImGui::DragFloat2("Size", glm::value_ptr(compTransform->m_Size), 0.1f);
				}

				ImGui::Separator();

				GrizzlyBear::Addon::imgui_cheader("Component: Mesh2D", [&]() -> std::pair<bool*, bool*>
				{
					if (m_ComponentMesh2DEnabled != scene->has_component<GrizzlyBear::CompMesh2D>(GrizzlyBear::MemoryType::Dynamic, m_EntityHandle))
					{
						if (m_ComponentMesh2DEnabled)
						{
							scene->attach_component<GrizzlyBear::CompMesh2D>(m_EntityHandle);
						}
						else
						{
							scene->detach_component<GrizzlyBear::CompMesh2D>(m_EntityHandle);
						}
					}

					return { &m_ComponentMesh2DOpen, &m_ComponentMesh2DEnabled };
				}, [&]() -> void
				{

					//Main Comp Section

					GrizzlyBear::CompMesh2D* mesh2D = scene->get_component<GrizzlyBear::CompMesh2D>(GrizzlyBear::MemoryType::Dynamic, m_EntityHandle);

					ImGui::Separator();

					ImGui::PushItemWidth(ImGui::GetWindowSize().x * 0.5f);

					ImGui::Text("Tiling: ");

					ImGui::SameLine();

					ImGui::DragFloat("##tiling", &mesh2D->m_Material.TILING_FACTOR, 1.0f, 0.0f, 0.0f, "%.0f", 1.0f);

					ImGui::Text("Color:  ");

					ImGui::SameLine();

					ImGui::ColorEdit4("##quad_color", glm::value_ptr(mesh2D->m_Material.TINT));

					ImGui::Text("Streamable: ");

					ImGui::SameLine();

					ImGui::Checkbox("##texture_streamable_mesh", &mesh2D->m_Material.STREAMABLE);

					ImGui::Text("Path:   ");

					ImGui::SameLine();

					if (ImGui::InputText("##texture_path_mesh", &m_Mesh2DTextureFilePath, ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::IsItemDeactivatedAfterEdit())
					{
						mesh2D->m_Material.TEXTURE_FILEPATH = m_Mesh2DTextureFilePath;
					}

					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AM_PushFile"))
						{
							GrizzlyBear::FilePath path((const char*)payload->Data);

							if (!path.is_directory())
							{
								if (path.get_extension() == "tga" || path.get_extension() == "png" || path.get_extension() == "jpg")
								{
									mesh2D->m_Material.TEXTURE_FILEPATH = path.get_path();
									m_Mesh2DTextureFilePath = path.get_path();
								}
							}
						}
					}

					ImGui::PopItemWidth();

					ImGui::SameLine();

					if (ImGui::Button("Browse"))
					{
						if (GrizzlyBear::Platform::get_file(m_Mesh2DTextureFilePath, "Select a Texture", ""))
						{
							mesh2D->m_Material.TEXTURE_FILEPATH = m_Mesh2DTextureFilePath;
						}
					}

					//Animation Section
					/*
					if (ImGui::CollapsingHeader("Animation: SpriteSheet"))
					{
						ImGui::Indent();

						GrizzlyBear::Addon::imgui_grey_out(GrizzlyBear::Addon::imgui_checkbox("###enable_animation", &m_Mesh2DAnimationEnabled), [&]()
						{
							mesh2D->m_AnimatedTex.m_Attached = m_Mesh2DAnimationEnabled;

							ImGui::PushItemWidth(ImGui::GetWindowSize().x * 0.5f);

							ImGui::Text("Path:   ");

							ImGui::SameLine();

							if (ImGui::InputText("##texture_path_sprite_sheet", &m_Mesh2DSpriteSheetFilePath, ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::IsItemDeactivatedAfterEdit)
							{
								mesh2D->m_AnimatedTex.m_Animation.set_sprite_sheet(m_Mesh2DSpriteSheetFilePath);
							}

							ImGui::PopItemWidth();

							ImGui::SameLine();

							if (ImGui::Button("Browse"))
							{
								if (GrizzlyBear::Platform::get_file(m_Mesh2DSpriteSheetFilePath, "Select a Sprite Sheet", "SpriteSheets (*.JPG;*.JPEG;*.PNG;*.TGA;*.BMP;*.PSD;*.GIF;*.HDR;*.PIC) | *.JPG;*.JPEG;*.PNG;*.TGA;*.BMP;*.PSD;*.GIF;*.HDR;*.PIC"));
								{
									mesh2D->m_AnimatedTex.m_Animation.set_sprite_sheet(m_Mesh2DSpriteSheetFilePath);
								}
							}
						});
					}*/
				});

				GrizzlyBear::Addon::imgui_cheader("Component: Camera", [&]()->std::pair<bool*, bool*>
				{
					if (m_ComponentCameraEnabled != scene->has_component<GrizzlyBear::CompCamera>(GrizzlyBear::MemoryType::Dynamic, m_EntityHandle))
					{
						if (m_ComponentCameraEnabled)
						{
							scene->attach_component<GrizzlyBear::CompCamera>(m_EntityHandle);
						}
						else
						{
							scene->detach_component<GrizzlyBear::CompCamera>(m_EntityHandle);
						}
					}

					return { &m_ComponentCameraOpen, &m_ComponentCameraEnabled };

				}, [&]()
				{
					ImGui::Separator();

					GrizzlyBear::CompCamera* camera = scene->get_component<GrizzlyBear::CompCamera>(GrizzlyBear::MemoryType::Dynamic, m_EntityHandle);
				});

				GrizzlyBear::Addon::imgui_cheader("Component: Widget", [&]()->std::pair<bool*, bool*>
				{
					if (m_ComponentWidgetEnabled != scene->has_component<GrizzlyBear::CompWidget>(GrizzlyBear::MemoryType::Dynamic, m_EntityHandle))
					{
						if (m_ComponentWidgetEnabled)
						{
							scene->attach_component<GrizzlyBear::CompWidget>(m_EntityHandle);
						}
						else
						{
							scene->detach_component<GrizzlyBear::CompWidget>(m_EntityHandle);
						}
					}

					return { &m_ComponentWidgetOpen, &m_ComponentWidgetEnabled };

				}, [&]() -> void
				{
					GrizzlyBear::CompWidget* widget = scene->get_component<GrizzlyBear::CompWidget>(GrizzlyBear::MemoryType::Dynamic, m_EntityHandle);

					ImGui::Separator();

					ImGui::PushItemWidth(ImGui::GetWindowSize().x * 0.5f);

					ImGui::Text("Path:   ");

					ImGui::SameLine();

					if (ImGui::InputText("##texture_path_widget", &m_WidgetTextureFilePath, ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::IsItemDeactivatedAfterEdit())
					{
						widget->m_Texture = m_WidgetTextureFilePath;
					}

					ImGui::PopItemWidth();

					ImGui::SameLine();

					if (ImGui::Button("Browse"))
					{
						if (GrizzlyBear::Platform::get_file(m_WidgetTextureFilePath, "Select a Texture", "Textures (*.JPG;*.JPEG;*.PNG;*.TGA;*.BMP;*.PSD;*.GIF;*.HDR;*.PIC) | *.JPG;*.JPEG;*.PNG;*.TGA;*.BMP;*.PSD;*.GIF;*.HDR;*.PIC"))
						{
							widget->m_Texture = m_WidgetTextureFilePath;
						}
					}
				});

				GrizzlyBear::Addon::imgui_cheader("Component: Script", [&]()->std::pair<bool*, bool*>
				{
					if (m_ComponentScriptEnabled != scene->has_component<GrizzlyBear::CompScript>(GrizzlyBear::MemoryType::Dynamic, m_EntityHandle))
					{
						if (m_ComponentScriptEnabled)
						{
							scene->attach_component<GrizzlyBear::CompScript>(m_EntityHandle);
						}
						else
						{
							scene->detach_component<GrizzlyBear::CompScript>(m_EntityHandle);
						}
					}

					return { &m_ComponentScriptOpen, &m_ComponentScriptEnabled };

				}, [&]() -> void
				{
					ImGui::Separator();

					GrizzlyBear::CompScript* script = scene->get_component<GrizzlyBear::CompScript>(GrizzlyBear::MemoryType::Dynamic, m_EntityHandle);

					if (ImGui::InputText("##script_path", &m_ScriptFilePath, ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::IsItemDeactivatedAfterEdit())
					{
						script->FILE_PATH = m_ScriptFilePath;
					}

					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("AM_PushFile"))
						{
							GrizzlyBear::FilePath path((const char*)payload->Data);

							if (!path.is_directory())
							{
								if (path.get_extension() == "lua")
								{
									script->FILE_PATH = path.get_path();
									m_ScriptFilePath = path.get_path();
								}
							}
						}
					}

					ImGui::SameLine();

					if (ImGui::Button("Simulate"))
					{
						m_SceneManager->get_scene()->start_simulation(m_EntityHandle.get_id());
					}

					ImGui::SameLine();

					if (ImGui::Button("Stop"))
					{
						m_SceneManager->get_scene()->stop_simulation(m_EntityHandle.get_id());
					}
				});

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		
	}

	void ObjectEditorWindow::reset_window()
	{
		m_EntityHandle = GrizzlyBear::EntityHandle::none();

		m_Mesh2DTextureFilePath = "";
		m_WidgetTextureFilePath = "";

		m_EntityName = "";
	}

}