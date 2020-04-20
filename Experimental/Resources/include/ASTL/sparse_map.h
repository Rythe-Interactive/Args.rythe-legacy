#pragma once

#include <vector>
#include <type_traits>
#include <algorithm>
#include <stdexcept>

namespace Args::stl
{
	// non hash based quick lookup contiguous map
	template <typename key_type, typename value_type, template<typename> typename dense_type = std::vector, template<typename> typename sparse_type = std::vector>
	class sparse_map
	{
		static_assert(std::is_unsigned_v<key_type>, "key_type must an unsigned type.");
	public:
		using self_type = sparse_map<key_type, value_type, dense_type, sparse_type>;
		using self_reference = self_type&;
		using self_const_reference = const self_type&;

		using size_type = std::size_t;

		using key_reference = key_type&;
		using key_const_reference = const key_type&;
		using key_pointer = key_type*;

		using value_reference = value_type&;
		using value_const_reference = const value_type&;
		using value_pointer = value_type*;

		struct value_container
		{
			key_type key;
			value_type value;

			value_container(key_type key, value_type value) : key(key), value(value) {}
			value_container(key_type key) : key(key) {}

			operator value_type() const { return value; } // gets copy
			operator value_reference() { return value; }
			operator value_pointer() { return &value; }
			operator value_const_reference() const { return value; }
			bool operator==(value_const_reference rhs) const { return value == rhs; }
			bool operator!=(value_const_reference rhs) const { return value != rhs; }
			value_container& operator= (value_const_reference source) { value = source; return *this; }
		};

		using sparse_container = sparse_type<size_type>;
		using dense_container = dense_type<value_container>;

		using iterator = typename dense_container::iterator;
		using const_iterator = typename dense_container::const_iterator;

	private:
		dense_container m_dense;
		sparse_container m_sparse;

		size_type m_size = 0;
		size_type m_capacity = 0;

	public:
		[[nodiscard]] iterator begin() { return m_dense.begin(); }
		[[nodiscard]] const_iterator begin() const { return m_dense.cbegin(); }

		[[nodiscard]] iterator end() { return m_dense.begin() + m_size; }
		[[nodiscard]] const_iterator end() const { return m_dense.cbegin() + m_size; }

		[[nodiscard]] size_type size() const noexcept { return m_size; }

		[[nodiscard]] size_type capacity() const noexcept { return m_capacity; }

		[[nodiscard]] bool empty() const noexcept { return m_size == 0; }

		void clear() noexcept { m_size = 0; }
		void reserve(size_type size)
		{
			if (size > m_capacity)
			{
				m_dense.resize(size, 0);
				m_sparse.resize(size, 0);
				m_capacity = size;
			}
		}

		size_type count(key_const_reference key) const
		{
			return contains(key);
		}
		size_type count(key_type&& key) const
		{
			return contains(key);
		}

		bool contains(key_const_reference key) const
		{
			return key < m_capacity&& m_sparse[key] < m_size&& m_dense[m_sparse[key]].key == key;
		}
		bool contains(key_type&& key) const
		{
			return key < m_capacity&& m_sparse[key] < m_size&& m_dense[m_sparse[key]].key == key;
		}

		iterator find(value_const_reference val)
		{
			return std::find(begin(), end(), val);
		}
		const_iterator find(value_const_reference val) const
		{
			return std::find(begin(), end(), val);
		}

		std::pair<iterator, bool> insert(key_const_reference key, value_const_reference val)
		{
			if (!contains(key))
			{
				if (key >= m_capacity)
					reserve(key + 1);

				auto itr = m_dense.begin() + m_size;
				*itr = std::move(value_container(key, val));
				m_sparse[key] = (key_type)m_size;
				++m_size;
				return std::make_pair(itr, true);
			}
			return std::make_pair(m_dense.end(), false);
		}
		std::pair<iterator, bool> insert(key_type&& key, value_const_reference val)
		{
			if (!contains(key))
			{
				if (key >= m_capacity)
					reserve(key + 1);

				auto itr = m_dense.begin() + m_size;
				*itr = std::move(value_container(key, val));
				m_sparse[key] = (key_type)m_size;
				++m_size;
				return std::make_pair(itr, true);
			}
			return std::make_pair(m_dense.end(), false);
		}
		std::pair<iterator, bool> insert(key_const_reference key, value_type&& val)
		{
			if (!contains(key))
			{
				if (key >= m_capacity)
					reserve(key + 1);

				auto itr = m_dense.begin() + m_size;
				*itr = std::move(value_container(key, val));
				m_sparse[key] = (key_type)m_size;
				++m_size;
				return std::make_pair(itr, true);
			}
			return std::make_pair(m_dense.end(), false);
		}
		std::pair<iterator, bool> insert(key_type&& key, value_type&& val)
		{
			if (!contains(key))
			{
				if (key >= m_capacity)
					reserve(key + 1);

				auto itr = m_dense.begin() + m_size;
				*itr = std::move(value_container(key, val));
				m_sparse[key] = (key_type)m_size;
				++m_size;
				return std::make_pair(itr, true);
			}
			return std::make_pair(m_dense.end(), false);
		}

		value_reference operator[](key_const_reference key)
		{
			if (!contains(key))
				throw std::out_of_range("Sparse map does not contain this key.");

			/// Auto add feature that the STL map and unordered_map have. Personally I hate it. But if you want it, here you go.
			//if (!contains(key))
			//{
			//	if (key >= m_capacity)
			//		reserve(key + 1);
			//
			//	auto itr = m_dense.begin() + m_size;
			//	*itr = std::move(value_container(key));
			//	m_sparse[key] = (key_type)m_size;
			//	++m_size;
			//}

			return m_dense[m_sparse[key]].value;
		}
		const value_reference operator[](key_const_reference key) const
		{
			if (!contains(key))
				throw std::out_of_range("Sparse map does not contain this key and is non modifiable.");

			return m_dense[m_sparse[key]].value;
		}

		size_type erase(key_const_reference key)
		{
			if (contains(key))
			{
				m_dense[m_sparse[key]] = m_dense[m_size - 1];
				m_sparse[m_dense[m_size - 1]] = m_sparse[key];
				--m_size;
				return true;
			}
			return false;
		}
	};
}
