#pragma once

#include <GrizzlyBear/Core/BaseDefines.h>

#include "GrizzlyBear/Scene/EntityManager.h"

namespace GrizzlyBear {

	struct Transform2D
	{
		glm::vec3 POSITION = { 0.0f, 0.0f, 0.0f };
		float ROTATION = 0.0f;
		glm::vec2 SCALE = { 0.0f, 0.0f };
	};

	class Scene;
	class EntityHandle;

	class Entity2D
	{
	public:

		Entity2D() : m_Scene(nullptr), m_Handle(0) {}

		virtual ~Entity2D() = default;

		void set_scene(Scene* newLink, EntityHandle myHandle);
		void _sync();
		void _fetch();

		virtual void on_spawn() = 0;

		virtual void on_update(Timestep ts) = 0;

		virtual void on_delete() = 0;

		virtual bool on_key(KeyEvent ev, KeyCode code) = 0;
		virtual bool on_mouse(MouseEvent ev, MouseCode code) = 0;

		bool is_key_pressed(int code);
		bool is_mouse_pressed(int code);

		MaterialProps2D& get_material();
		Animation2D& get_animation();

		bool is_visible();
		bool is_widget();
		bool is_animating();

		void enable_events(bool enable = true);
		void make_visible(bool enable = true);
		void make_widget(bool enable = true);

		void play_animation(const Animation2D& anim);
		void play_animation(const Gstring& globalAnimName);

	public:
		Transform2D m_Transform;
		Scene* m_Scene;
		EntityHandle m_Handle;
		bool m_GetEvents = false;;
	};

}


