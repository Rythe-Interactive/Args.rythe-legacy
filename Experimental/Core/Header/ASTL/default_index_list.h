#pragma once
#include <vector>
namespace Args::stl
{
	template<typename value_type, size_t default_index = 0, template<typename> typename container_type = std::vector>
	struct default_index_list
	{
		using container = container_type<value_type>;
		container values;

		value_type& operator[](size_t key) { return values[key]; }
		const value_type& operator[](size_t key) const { return values[key]; }

		bool empty() { return values.empty(); }

		operator container() const { return values; } // gets copy
		operator container& () { return values; }
		operator container* () { return &values; }
		operator const container& () const { return values; }

		operator value_type() const { return values[default_index]; } // gets copy
		operator value_type& () { return values[default_index]; }
		operator value_type* () { return &values[default_index]; }
		operator const value_type& () const { return values[default_index]; }
	};
}