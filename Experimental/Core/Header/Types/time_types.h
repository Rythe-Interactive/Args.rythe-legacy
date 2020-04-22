#pragma once

namespace Args
{
	using time32 = float;
	using time64 = double;

	using hours = time32;
	using minutes = time32;
	using seconds = time32;
	using milli_seconds = time32;
	using micro_seconds = time32;

	using fast_time = time32;
	using fast_hours = fast_time;
	using fast_minutes = fast_time;
	using fast_seconds = fast_time;
	using fast_milli_seconds = fast_time;
	using fast_micro_seconds = fast_time;

	using precise_time = time64;
	using precise_hours = precise_time;
	using precise_minutes = precise_time;
	using precise_seconds = precise_time;
	using precise_milli_seconds = precise_time;
	using precise_micro_seconds = precise_time;
}