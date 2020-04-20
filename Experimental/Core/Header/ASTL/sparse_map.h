//#pragma once
//
//#include <unordered_map>
//#include <vector>
//#include <type_traits>
//#include <algorithm>
//
//namespace Args::stl
//{
//	// non hash based quick lookup contiguous map
//	template <typename key_type, typename value_type, template<typename> typename dense_type = std::vector, template<typename> typename sparse_type = std::vector, template<typename, typename> typename map_type = std::unordered_map>
//	class sparse_map
//	{
//	public:
//		using self_type = sparse_map<key_type, value_type, dense_type, sparse_type, map_type>;
//		using self_reference = self_type&;
//		using self_const_reference = const self_type&;
//
//		using size_type = std::size_t;
//
//		using key_reference = key_type&;
//		using key_const_reference = const key_type&;
//		using key_pointer = key_type*;
//
//		using value_reference = value_type&;
//		using value_const_reference = const value_type&;
//		using value_pointer = value_type*;
//
//		struct value_container
//		{
//			size_type index;
//			value_type value;
//
//			value_container(value_type value, size_type index) : value(value), index(index) {}
//
//			operator value_type() const { return value; } // gets copy
//			operator value_reference() { return value; }
//			operator value_pointer() { return &value; }
//			operator value_const_reference() const { return value; }
//		};
//
//		struct retainable
//		{
//			key_type key;
//			self_const_reference owner;
//			retainable(self_const_reference owner, key_const_reference key) : owner(owner), key(key) {}
//
//			operator value_type() const { return owner[key]; } // gets copy
//			operator value_reference() { return owner[key]; }
//			operator value_pointer() { return &owner[key]; }
//			operator value_const_reference() const { return owner[key]; }
//		};
//
//		using key_map = map_type<key_type, size_type>;
//		using sparse_container = sparse_type<size_type>;
//		using dense_container = dense_type<value_container>;
//
//		using iterator = typename dense_container::iterator;
//		using const_iterator = typename dense_container::const_iterator;
//
//	private:
//		dense_container m_dense;
//		sparse_container m_sparse;
//
//		size_type m_size = 0;
//		size_type m_capacity = 0;
//
//	public:
//		[[nodiscard]] iterator begin() { return m_dense.begin(); }
//		[[nodiscard]] const_iterator begin() const { return m_dense.cbegin(); }
//
//		[[nodiscard]] iterator end() { return m_dense.begin() + m_size; }
//		[[nodiscard]] const_iterator end() const { return m_dense.cbegin() + m_size; }
//
//		[[nodiscard]] size_type size() const noexcept { return m_size; }
//
//		[[nodiscard]] size_type capacity() const noexcept { return m_capacity; }
//
//		[[nodiscard]] bool empty() const noexcept { return m_size == 0; }
//
//		void clear() noexcept { m_size = 0; }
//		void reserve(size_type size)
//		{
//			if (size > m_capacity)
//			{
//				m_dense.resize(size, 0);
//				m_sparse.resize(size, 0);
//				m_capacity = size;
//			}
//		}
//
//		size_type count(const_reference val) const
//		{
//			return contains(val);
//		}
//		size_type count(value_type&& val) const
//		{
//			return contains(val);
//		}
//
//		bool contains(const_reference val) const
//		{
//			return val < m_capacity&& m_sparse[val] < m_size&& m_dense[m_sparse[val]] == val;
//		}
//		bool contains(value_type&& val) const
//		{
//			return val < m_capacity&&
//				m_sparse[val] < m_size&&
//				m_dense[m_sparse[val]] == val;
//		}
//
//		iterator find(const_reference val)
//		{
//			return std::find(m_dense.begin(), m_dense.end(), val);
//		}
//		const_iterator find(const_reference val) const
//		{
//			return std::find(m_dense.begin(), m_dense.end(), val);
//		}
//
//		std::pair<iterator, bool> insert(const_reference val)
//		{
//			if (!contains(val))
//			{
//				if (val >= m_capacity)
//					reserve(val + 1);
//
//				auto itr = m_dense.begin() + m_size;
//				*itr = val;
//				m_sparse[val] = (value_type)m_size;
//				++m_size;
//				return std::make_pair(itr, true);
//			}
//			return std::make_pair(m_dense.end(), false);
//		}
//		std::pair<iterator, bool> insert(value_type&& val)
//		{
//			if (!contains(val))
//			{
//				if (val >= m_capacity)
//					reserve(val + 1);
//
//				auto itr = m_dense.begin() + m_size;
//				*itr = val;
//				m_sparse[val] = (value_type)m_size;
//				++m_size;
//				return std::make_pair(itr, true);
//			}
//			return std::make_pair(m_dense.end(), false);
//		}
//
//		size_type erase(const_reference val)
//		{
//			if (contains(val))
//			{
//				m_dense[m_sparse[val]] = m_dense[m_size - 1];
//				m_sparse[m_dense[m_size - 1]] = m_sparse[val];
//				--m_size;
//				return true;
//			}
//			return false;
//		}
//	};
//}
