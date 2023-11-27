#include <gbpch.h>

#include "Entity.h"

#include "GrizzlyBear/Scene/Scene.h"

namespace GrizzlyBear {

	void Entity2D::set_scene(Scene* newLink, EntityHandle myHandle)
	{
		m_Scene = newLink;
		m_Handle = myHandle;
	}

	void Entity2D::_sync()
	{
		auto comp = m_Scene->get_component<CompTransformation>(MemoryType::Dynamic, m_Handle);
		comp->m_Position = m_Transform.POSITION;
		comp->m_Rotation.x = m_Transform.ROTATION;
		comp->m_Size.x = m_Transform.SCALE.x;
		comp->m_Size.y = m_Transform.SCALE.y;
	}

	void Entity2D::_fetch()
	{
		auto comp = m_Scene->get_component<CompTransformation>(MemoryType::Dynamic, m_Handle);
		m_Transform.POSITION = comp->m_Position;
		m_Transform.ROTATION = comp->m_Rotation.x;
		m_Transform.SCALE.x = comp->m_Size.x;
		m_Transform.SCALE.y = comp->m_Size.y;
	}

	bool Entity2D::is_key_pressed(int code)
	{
		return m_Scene->get_window()->is_key_pressed(code);
	}

	bool Entity2D::is_mouse_pressed(int code)
	{
		return m_Scene->get_window()->is_mouse_pressed(code);
	}

	MaterialProps2D& Entity2D::get_material()
	{
		return m_Scene->get_component<CompSprite>(MemoryType::Dynamic, m_Handle)->MATERIAL;
	}

	Animation2D& Entity2D::get_animation()
	{
		return m_Scene->get_animator_2d()->get_animation_instance(m_Handle.get_id());
	}

	bool Entity2D::is_visible()
	{
		return m_Scene->has_component<CompSprite>(MemoryType::Dynamic, m_Handle);
	}

	bool Entity2D::is_widget()
	{
		return m_Scene->get_component<CompSprite>(MemoryType::Dynamic, m_Handle)->HUD;
	}

	bool Entity2D::is_animating()
	{
		return m_Scene->get_animator_2d()->is_active(m_Handle.get_id());
	}

	void Entity2D::enable_events(bool enable)
	{
		m_GetEvents = enable;
	}

	void Entity2D::make_visible(bool enable)
	{
		if (enable)
			m_Scene->attach_component<CompSprite>(m_Handle);
		else
			m_Scene->detach_component<CompSprite>(m_Handle);
	}

	void Entity2D::make_widget(bool enable)
	{
		m_Scene->get_component<CompSprite>(MemoryType::Dynamic, m_Handle)->HUD = enable;
	}

	void Entity2D::play_animation(const Animation2D& anim)
	{
		m_Scene->get_animator_2d()->play_animation(anim, m_Handle.get_id());
	}

	void Entity2D::play_animation(const Gstring& globalAnimName)
	{
		m_Scene->get_animator_2d()->play_animation(globalAnimName, m_Handle.get_id());
	}

}