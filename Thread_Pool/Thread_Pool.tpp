#include "Thread_Pool.hpp"
using namespace threads;

 template <typename T, typename Container>
constexpr Thread_Pool<T, Container>::Thread_Pool(Iterator begin, Iterator end, Tool_ptr tool) {
    set_thread_count();
    set_begin_end(begin, end);
    set_ranges();
    m_callable.reset_tool(tool);
}

template <typename T, typename Container>
constexpr void Thread_Pool<T, Container>::change_tool(Iterator begin, Iterator end, Tool_ptr tool, Tool_Type tool_type) {
    assert(tool != nullptr && "Provided tool must not be nullptr\n");
    m_callable.reset_tool(tool);
    m_tool_type = tool_type;
    m_begin = begin;
    m_end = end;
}


 template <typename T, typename Container>
constexpr void Thread_Pool<T, Container>::set_thread_count() {
    m_thread_count = sysconf(_SC_NPROCESSORS_ONLN);
}


 template <typename T, typename Container>
constexpr void Thread_Pool<T, Container>::set_begin_end(Iterator begin, Iterator end) {
    m_begin = begin;
    m_end = end;
}

/*
    this function is resposnible to decide the range size of each thread
    but here I also counted the problem connected with cache line
    in standard platform the size of a cache line is 64 bytes
    so here I also counted that 64 bytes
*/
 template <typename T, typename Container>
constexpr void Thread_Pool<T, Container>::set_ranges() {
      auto num_of_cache_lines = get_n_of_cache_lines();
      auto num_of_elems_line = get_n_of_elems_line();
      divide_among_cores(num_of_cache_lines, num_of_elems_line);
}

//deciding the number of cache lines in my range
 template <typename T, typename Container>
constexpr int Thread_Pool<T, Container>::get_n_of_cache_lines() const {
    auto cache_line_size{64};
    auto num_of_elems_line = cache_line_size / sizeof(T);
    auto size = m_end - m_begin;
    int num_of_lines_in_container = (size % num_of_elems_line) ? (size / num_of_elems_line) + 1 : (size / num_of_elems_line);

    return num_of_lines_in_container;
}

 template <typename T, typename Container>
constexpr int Thread_Pool<T, Container>::get_n_of_elems_line() const {
    auto cache_line_size{64};
    auto num_of_elems_line = cache_line_size / sizeof(T);
    return num_of_elems_line;
}


 template <typename T, typename Container>
constexpr void Thread_Pool<T, Container>::divide_among_cores(int n_lines, int n_elems) {
    std::vector<int> range_sizes(m_thread_count, 0);
    size_t elem_count{size_t(m_end - m_begin)};

    while (n_lines > 0) {
        for (size_t i{0}; i < m_thread_count && n_lines; ++i) {
            range_sizes[i]++;
            --n_lines;
        }
    }
     range_sizes.erase(
        std::remove(range_sizes.begin(), range_sizes.end(), 0), 
        range_sizes.end()
    );
    m_thread_count = range_sizes.size();
    
    size_t first{0};
    size_t last{0};
    for (size_t i{0}; i < range_sizes.size(); ++i) {
        last = first + range_sizes[i] * n_elems;
        last = (last < elem_count) ? last : elem_count - 1;
        m_ranges.push_back({first, last});
        first = ++last;
    }
}


 template <typename T, typename Container>
constexpr void Thread_Pool<T, Container>::action() {
    if (m_tool_type == Tool_Type::Recursive) {
        run_in_recursive_mode(); 
        return;
    }
    run_in_linear_mode();     
}


 template <typename T, typename Container>
 constexpr void Thread_Pool<T, Container>::run_in_linear_mode() {
    Thread_List threads;
    create_threads(threads);
    join_threads(threads);
 }

 template <typename T, typename Container>
 constexpr void Thread_Pool<T, Container>::run_in_recursive_mode() {
    //remember previous state
    auto tmp_thread_count = m_thread_count;
    auto ranges = m_ranges;
    while (m_thread_count > 0) {
            Thread_List threads;
            create_threads(threads);
            join_threads(threads);
            merge_ranges();
    }

    restate(tmp_thread_count, ranges);
 }

 template <typename T, typename Container>
 constexpr void Thread_Pool<T, Container>::restate(size_t thread_count, List ranges) {
     m_thread_count = thread_count;
     m_ranges = ranges;
 }

 template <typename T, typename Container>
constexpr void Thread_Pool<T, Container>::merge_ranges() {
    std::vector<Range> new_ranges;
    size_t i{0};
    if (m_ranges.size() == 1) {
        m_thread_count = 0;
        return;
    }

    for (; i < m_ranges.size() - 1; i += 2) {
        const auto[r1, l1] = m_ranges[i];
        const auto[r2, l2] = m_ranges[i + 1];
        new_ranges.push_back({r1, l2});
    }
    if (i < m_ranges.size()) {
        new_ranges.push_back(m_ranges[i]); //when count is odd one is left alone
    }
    assert(new_ranges.size() > 0 && "Ranges must not be empty\n");
    m_thread_count = (new_ranges.size() % 2 && new_ranges.size() > 1) ? new_ranges.size() - 1 : new_ranges.size();
    std::swap(new_ranges, m_ranges);
}

 template <typename T, typename Container>
constexpr void Thread_Pool<T, Container>::create_threads(Thread_List& threads) {
    for (size_t i{0}; i < m_thread_count; ++i) {
        const auto[right, left] = m_ranges[i];
        auto begin = m_begin + right;
        auto end = m_begin + left + 1;
        threads.push_back(std::thread((m_callable), begin, end));
    }
}

 template <typename T, typename Container>
constexpr void Thread_Pool<T, Container>::join_threads(Thread_List& threads) const{
    for (size_t i{0}; i < threads.size(); ++i) {
        threads[i].join();
    }
}


