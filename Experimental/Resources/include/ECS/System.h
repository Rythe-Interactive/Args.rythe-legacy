#pragma once
#include <Scheduling/process.h>
#include <Types/identification.h>
#include <map>
#include <memory>

namespace Args
{
	class Engine;

	class SystemBase
	{

	};

	template<class SystemType>
	class System : public SystemBase
	{
	protected:
		Engine* engine;

	public:
		std::map<process_id, std::unique_ptr<process>> processes;

		System() = default;
		System(Engine* engine) : engine(engine){}

		virtual void init() = 0;
		
	};
}