#pragma once
#include "Merge_Sort.hpp"
#include "File_Creator.hpp"
#include <memory>
#include <thread>
#include <utility>
#include <functional>
#include "Tool.hpp"
#include "Callable.hpp"
#include <unistd.h>

namespace threads {
    template <typename T, typename Container = std::vector<T>>
    class Thread_Pool {
        public:
            enum class Tool_Type{Recursive = 1, Linear};
        private:
            using Iterator = typename Container::iterator;
            using Tool_ptr = task::Tool<T, Container>*;
            using Range = std::pair<size_t, size_t>;
            using List = std::vector<Range>;
            using Thread_List = std::vector<std::thread>;
        public:
            constexpr Thread_Pool(Iterator, Iterator, Tool_ptr = nullptr);
            constexpr void action();
            constexpr void change_tool(Iterator, Iterator, Tool_ptr, Tool_Type);
        private:
            constexpr void run_in_linear_mode();
            constexpr void run_in_recursive_mode();
            constexpr void set_thread_count();
            constexpr void set_begin_end(Iterator, Iterator);
            constexpr void set_ranges();
            constexpr int get_n_of_cache_lines() const;
            constexpr int get_n_of_elems_line() const;
            constexpr void divide_among_cores(int, int);
            constexpr void merge_ranges();
            constexpr void create_threads(Thread_List&);
            constexpr void join_threads(Thread_List&) const;
            constexpr void restate(size_t, List);
        private:
            size_t m_thread_count{2};
            Iterator m_begin{Iterator()};
            Iterator m_end{Iterator()};
            List m_ranges{};
            task::Collable<T, Container> m_callable;
            Tool_Type m_tool_type{Tool_Type::Recursive};
    };
    #include "Thread_Pool.tpp"
} //namesapce threads