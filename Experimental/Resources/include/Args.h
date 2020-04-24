#pragma once

#include <ASTL/default_index_list.h>
#include <ASTL/delegate.h>
#include <ASTL/sparse_map.h>
#include <ASTL/sparse_set.h>

#include <ECS/component.h>
#include <ECS/component_family.h>
#include <ECS/component_query.h>
#include <ECS/ecs_containers.h>
#include <ECS/entity.h>
#include <ECS/EntityComponentSystem.h>
#include <ECS/System.h>

#include <Engine/Engine.h>

#include <Logging/Console.h>
#include <Logging/Debug.h>

#include <Scheduling/job.h>
#include <Scheduling/operation.h>
#include <Scheduling/process.h>
#include <Scheduling/process_group.h>
#include <Scheduling/Scheduler.h>

#include <Timing/clock.h>
#include <Timing/time.h>

#include <Types/identification.h>
#include <Types/primitives.h>
#include <Types/prioritization.h>
#include <Types/SFINAE.h>
#include <Types/time_types.h>