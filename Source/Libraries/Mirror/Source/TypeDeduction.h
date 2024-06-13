#pragma once

// From: https://stackoverflow.com/questions/9407367/determine-if-a-type-is-an-stl-container-at-compile-time/31105859#31105859

#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <type_traits>

// Specialize a type for all of the STL containers.
namespace is_stl_container_impl {
	template <typename T>       struct is_stl_container :std::false_type {};
	template <typename T, std::size_t N> struct is_stl_container<std::array    <T, N>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::vector            <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::deque             <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::list              <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::forward_list      <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::set               <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::multiset          <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::map               <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::multimap          <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::unordered_set     <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::unordered_multiset<Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::unordered_map     <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::unordered_multimap<Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::stack             <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::queue             <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_container<std::priority_queue    <Args...>> :std::true_type {};
}

// #TODO Look to reduce redundancy with is_stl_container_impl. Maybe include vector check in container check
namespace is_stl_vector_impl {
	template <typename T>       struct is_stl_vector :std::false_type {};
	template <typename... Args> struct is_stl_vector<std::vector <Args...>> :std::true_type {};
}

namespace is_stl_map_impl {
	template <typename T>       struct is_stl_map :std::false_type {};
	template <typename... Args> struct is_stl_map<std::map <Args...>> :std::true_type {};
	template <typename... Args> struct is_stl_map<std::unordered_map <Args...>> :std::true_type {};
}

namespace is_stl_pair_impl {
	template <typename T>       struct is_stl_pair :std::false_type {};
	template <typename... Args> struct is_stl_pair<std::pair <Args...>> :std::true_type {};
}

// Type trait to utilize the implementation type traits as well as decay the type
template <typename T> struct is_stl_container {
	static constexpr bool const value = is_stl_container_impl::is_stl_container<std::decay_t<T>>::value;
};

template <typename T> struct is_stl_vector {
	static constexpr bool const value = is_stl_vector_impl::is_stl_vector<std::decay_t<T>>::value;
};

template <typename T> struct is_stl_map {
	static constexpr bool const value = is_stl_map_impl::is_stl_map<std::decay_t<T>>::value;
};

template <typename T> struct is_stl_pair {
	static constexpr bool const value = is_stl_pair_impl::is_stl_pair<std::decay_t<T>>::value;
};
