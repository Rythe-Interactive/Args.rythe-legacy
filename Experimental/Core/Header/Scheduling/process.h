#pragma once
#include <Scheduling/operation.h>
#include <ECS/ecs_containers.h>
#include <Types/identification.h>
#include <Types/SFINAE.h>
#include <Timing/clock.h>
#include <Logging/Debug.h>
#include <memory>

namespace Args
{
	struct process
	{
		process_id id = invalid_id;

		fast_seconds interval = 0;
		fast_seconds timeBuffer = 0;
		bool fixedTimeStep = true;

		process() = default;

		template<size_type nameLength>
		process(const char(&name)[nameLength], fast_seconds interval) : interval(interval), fixedTimeStep(false), id(GetNameHash<nameLength>(name))
		{
			Debug::soft_assert(interval >= 0.01f, "warning, process WILL be updated at least %f times per frame!", 1.f / interval);
		}
		template<size_type nameLength>
		process(const char(&name)[nameLength]) : id(GetNameHash<nameLength>(name)) {}

		process(const process& source) : interval(source.interval), fixedTimeStep(source.fixedTimeStep), timeBuffer(source.timeBuffer), id(source.id) {}

		std::vector<std::unique_ptr<operation_base>> operations;

		clock<fast_time> clock;

		// triggers process update, returns whether the process has finished updating.
		bool step()
		{
			fast_seconds deltaTime = clock.End().seconds();

			if (!fixedTimeStep)
			{
				for (auto& operation : operations)
					operation->invoke(deltaTime);

				clock.Start();
				return true;
			}
			else
			{
				timeBuffer += deltaTime;

				if (timeBuffer >= interval)
				{
					timeBuffer -= interval;

					for (auto& operation : operations)
						operation->invoke(interval);
				}

				return timeBuffer < interval;
			}
		}

		process& operator=(const process& source)
		{
			*this = std::move(process(source));
			return *this;
		}

		template<typename operation_type, inherits_from<operation_type, operation_base> = 0>
		process& operator+=(const operation_type& operation)
		{
			operations.push_back(std::make_unique<operation_type>(operation));
			return *this;
		}

		template<typename operation_type, inherits_from<operation_type, operation_base> = 0>
		process& push_back(const operation_type& operation)
		{
			operations.push_back(std::make_unique<operation_type>(operation));
			return *this;
		}
	};
}