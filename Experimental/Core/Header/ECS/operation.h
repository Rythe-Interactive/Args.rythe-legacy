#pragma once
#include <ASTL/delegate.h>
#include <Types/time_types.h>
#include <Types/identification.h>
#include <Logging/Debug.h>
#include <ECS/component_query.h>

namespace Args
{
	struct operation_base
	{
		const id_type id;
	protected:
		id_type lastId = invalid_id;
	};

	template<template<typename> typename operation_type, typename... component_types>
	struct component_operation : public operation_base
	{
	protected:
		stl::multicast_delegate<void(fast_seconds, component_types...)> callbacks;

	public:
		static id_type id;

		component_operation() : operation_base::id(id) {}

		operation_type<component_types...>& operator+=(const stl::multicast_delegate<void(fast_seconds, component_types...)>& delegate)
		{
			callbacks += delegate;
		}
		operation_type<component_types...>& operator+=(const stl::delegate<void(fast_seconds, component_types...)>& delegate)
		{
			callbacks += delegate;
		}
		operation_type<component_types...>& operator+=(const component_operation<operation_type, component_types...>& other)
		{
			callbacks += other.callbacks;
		}


		operation_type<component_types...>& insert(const stl::multicast_delegate<void(fast_seconds, component_types...)>& delegate)
		{
			callbacks += delegate;
		}
		operation_type<component_types...>& insert(const stl::delegate<void(fast_seconds, component_types...)>& delegate)
		{
			callbacks += delegate;
		}
		operation_type<component_types...>& insert(const component_operation<operation_type, component_types...>& other)
		{
			callbacks += other.callbacks;
		}


		virtual bool operator()(fast_seconds delta_time) = 0;
		virtual bool invoke(fast_seconds delta_time) = 0;
	};

	template<template<typename> typename operation_type, typename... component_types>
	id_type component_operation<operation_type, component_types...>::id = ++lastId;

	template<typename... component_types>
	struct operation : public component_operation<operation, component_types...>
	{
		using component_operation<operation, component_types...>::component_operation;

		virtual bool operator()(fast_seconds delta_time, const component_query<component_types...>& query) override
		{			
			this->callbacks.invoke(delta_time);
			return true;
		}
		virtual bool invoke(fast_seconds delta_time) override
		{
			this->callbacks.invoke(delta_time);
			return true;
		}
	};

	template<typename... component_types>
	struct fixed_operation : public component_operation<operation, component_types...>
	{
		fast_seconds buffer = 0;
		fast_seconds interval;

		fixed_operation() = delete;
		fixed_operation(fast_seconds interval) : component_operation<operation, component_types...>(), interval(interval)
		{
			Debug::assert(interval >= 0.0001f, "Minimum interval size is 0.0001");
		}

		virtual bool operator()(fast_seconds delta_time) override
		{
			buffer += delta_time;

			if (buffer >= interval)
			{
				buffer -= interval;
				this->callbacks.invoke(interval);
			}

			return buffer < interval;
		}
		virtual bool invoke(fast_seconds delta_time) override
		{
			buffer += delta_time;

			if (buffer >= interval)
			{
				buffer -= interval;
				this->callbacks.invoke(interval);
			}

			return buffer < interval;
		}
	};
}