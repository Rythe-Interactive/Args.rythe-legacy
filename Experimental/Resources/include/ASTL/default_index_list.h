#pragma once
#include <vector>
#include <type_traits>

namespace Args::stl
{
	template<typename value_type, std::size_t default_index = 0, template<typename> typename container_type = std::vector>
	class default_index_list
	{
	public:
		using container = container_type<value_type>;

		using size_type = std::size_t;

		using reference = value_type&;
		using const_reference = const value_type&;

		using iterator = typename container::iterator;
		using const_iterator = typename container::const_iterator;

		reference operator[](size_type index) { return values[index]; }
		const_reference operator[](size_type index) const { return values[index]; }

		[[nodiscard]] iterator begin() { return values.begin(); }
		[[nodiscard]] const_iterator begin() const { return values.cbegin(); }

		[[nodiscard]] iterator end() { return values.end(); }
		[[nodiscard]] const_iterator end() const { return values.cend(); }

		//returns iterator at index, if index invalid throws our_of_range error
		[[nodiscard]] iterator at(size_type index)
		{
			if (index < values.size() && index >= 0)
				return values.begin() + index;
			else
				throw std::out_of_range("index out of range");
		}
		[[nodiscard]] const_iterator at(size_type index) const
		{
			if (index < values.size() && index >= 0)
				return values.cbegin() + index;
			else
				throw std::out_of_range("index out of range");
		}

		[[nodiscard]] bool empty() const noexcept { return values.empty(); }
		[[nodiscard]] size_type size() const noexcept { return values.size(); }

		iterator find(const_reference val)
		{
			return std::find(values.begin(), values.end(), val);
		}
		const_iterator find(const_reference val) const
		{
			return std::find(values.begin(), values.end(), val);
		}

		void clear() noexcept { values.clear(); }

		iterator erase_at(size_type index) { return values.erase(at(index)); }
		iterator erase(iterator pos) { return values.erase(pos); }
		iterator erase(const_iterator pos) { return values.erase(pos); }
		iterator erase(iterator first, iterator last) { return values.erase(first, last); }
		iterator erase(const_iterator first, const_iterator last) { return values.erase(first, last); }

		iterator insert(iterator pos, const_reference val) { return values.insert(pos, val); }
		iterator insert(const_iterator pos, const_reference val) { return values.insert(pos, val); }
		iterator insert(iterator pos, reference& val) { return values.insert(pos, val); }
		iterator insert(const_iterator pos, reference& val) { return values.insert(pos, val); }

		template<typename... Arguments>
		iterator emplace(iterator pos, Arguments&&... arguments) { return values.emplace(pos, arguments...); }
		template<typename... Arguments>
		iterator emplace(const_iterator pos, Arguments&&... arguments) { return values.emplace(pos, arguments...); }

		void push_back(const_reference val) { values.push_back(val); }
		void push_back(reference& val) { values.push_back(val); }

		template<typename... Arguments>
		reference emplace_back(Arguments&&... arguments) { return values.emplace_back(arguments); }

		operator container() const { return values; } // gets copy
		operator container& () { return values; }
		operator container* () { return &values; }
		operator const container& () const { return values; }

		operator value_type() const { return values[default_index]; } // gets copy
		operator reference () { return values[default_index]; }
		operator value_type* () { return &values[default_index]; }
		operator const_reference () const { return values[default_index]; }

	private:
		container values;
	};
}