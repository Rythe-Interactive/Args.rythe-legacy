#pragma once
#include <unordered_map>
#include <unordered_set>
#include <Scheduling/process.h>
#include <Types/identification.h>

namespace Args
{
	class Engine;
	class process_group
	{
		std::unordered_map<process_id, process> processes;

		Engine* engine;

		process_group() = default;
		process_group(Engine* engine) : engine(engine) {}

		void execute()
		{
			std::unordered_set<process_id> finishedProcesses;
			do
			{
				for (auto process : processes)
					if (!finishedProcesses.count(process.first))
						if (process.second.step())
							finishedProcesses.insert(process.first);
			} while (finishedProcesses.size() != processes.size());
		}

		process_group& operator+=(const process& process)
		{
			processes[process.id] = process;
			return *this;
		}

		process_group& insert(const process& process)
		{
			processes[process.id] = process;
			return *this;
		}

		process_group& operator-=(const process& process)
		{
			processes.erase(process.id);
			return *this;
		}

		process_group& erase(const process& process)
		{
			processes.erase(process.id);
			return *this;
		}

		template<size_type nameLength>
		process_group& operator-=(const char(&name)[nameLength])
		{
			processes.erase(GetNameHash<nameLength>(name));
			return *this;
		}

		template<size_type nameLength>
		process_group& erase(const char(&name)[nameLength])
		{
			processes.erase(GetNameHash<nameLength>(name));
			return *this;
		}
	};
}