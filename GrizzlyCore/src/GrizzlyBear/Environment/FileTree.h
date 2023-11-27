#pragma once

/*
#include <unordered_map>

#include "GrizzlyBear/Core/BaseDefines.h"
#include "GrizzlyBear/Core/Log.h"
#include "GrizzlyBear/Core/FileIO.h"


namespace GrizzlyBear {

	struct EnvNode
	{
		EnvNode(bool primary = false, bool loaded = false, bool leaf = false, int reserve = 16)
			: CHILDS(reserve), PRIMARY(primary), LOADED(false), LEAF(false) {}

		std::unordered_map<Gstring, size_t> CHILDS;
		bool PRIMARY;
		bool LOADED;
		bool LEAF;
	};

	struct FileTree
	{
	public:
		FileTree(const Gstring& root)
			: m_IdCounter(0), m_Ids(), m_Nodes(1024)
		{

		}

		void add_node(const FilePath& path)
		{
			if (path.is_valid_path())
			{
				EnvNode& curr = m_Nodes[0];

				for (int i = 0; i < path.get_depth(); i++)
				{
					if (i == path.get_depth() - 1)
					{
						curr.CHILDS[path[i]];
					}

					if (curr.CHILDS.contains(path[i]))
					{
						curr = m_Nodes[curr.CHILDS[path[i]]];
					}
					else
					{
						return;
					}
				}
			}
		}

		size_t reg_node(EnvNode node)
		{
			if (m_Ids.empty())
			{
				m_Nodes.emplace(m_Nodes.begin() + (*m_Ids.begin()), node);
				size_t ret = *m_Ids.begin();
				m_Ids.erase(m_Ids.begin());
				return ret;
			}
			else
			{
				m_Nodes.push_back(node);
				return m_Nodes.size() - 1;
			}
		}

		bool load_node(const FilePath& path, size_t id)
		{
			if (path.is_valid_path())
			{
				if (path.is_directory() && !m_Nodes[id].LEAF)
				{
					auto paths = FileIO::get_things_in_dir_rel(FileIO::open(path));
					m_Nodes.push_back(EnvNode(true, true, false, paths.size()));

					size_t current = id;

					for (RelativePath& p : paths)
					{
						Gstring last = p.m_FullPath[p.m_FullPath.get_depth() - 1];

						if (last.size() + 1 > 256)
						{
							GRIZZLY_CORE_LOG(LogLevel::Error, "Module: Environment, Message: Invalid path length {0}", last.size() + 1);
							continue;
						}

						bool dir = p.m_FullPath.is_directory();

						reg_node(EnvNode(false, !dir, !dir));
					}

					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}

	private:
		size_t m_IdCounter = 0;
		std::set<size_t> m_Ids;
		std::vector<EnvNode> m_Nodes;

	};
}

*/
