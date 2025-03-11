#pragma once
#include "Merge_Sort.hpp"
using namespace task;

template <typename T, typename Container>
constexpr void Merge_Sort<T, Container>::operator()(Iterator begin, Iterator end) {
    set_begin_end(begin, end);
    set_size(begin, end);

    //start to sort
    divide_comp_merge(0, m_size - 1);
}


template <typename T, typename Container>
constexpr void Merge_Sort<T, Container>::set_begin_end(Iterator begin, Iterator end) {
        m_begin = begin;
        m_end = end;
}

template <typename T, typename Container>
constexpr void Merge_Sort<T, Container>::set_size(Iterator begin, Iterator end) {
        assert(begin <= end && "Invalid Range\n");
        if (begin == end) {
                return;
        }
        auto diff = end - begin;
        m_size = diff;
}

template <typename T, typename Container>
constexpr void Merge_Sort<T, Container>::divide_comp_merge(size_t first, size_t last) {
        //base case 1 element
        if (first == last) {
                return;
        }
        auto mid = (first + last) / 2;
        Range right{first, mid};
        Range left{mid + 1, last};

        //Divide_&_merge
        divide_comp_merge(first, mid);
        divide_comp_merge(mid + 1, last);
        merge(right, left);
}

template <typename T, typename Container>
constexpr void Merge_Sort<T, Container>::merge(Range right, Range left) {
        auto[r_1, l_1] = right;
        auto[r_2, l_2] = left;
        Range full_range{r_1, l_2};
        std::vector<T> tmp(l_2 - r_1 + 1);

        size_t i{0};
        while (r_1 <= l_1 && r_2 <= l_2) {
                if (*(m_begin + r_1) < *(m_begin + r_2)) {
                        tmp[i++] = *(m_begin + r_1);
                        ++r_1;
                        continue;
                }
                tmp[i++] = *(m_begin + r_2);
                ++r_2;
        }

        while (r_1 <= l_1) {
                tmp[i++] = *(m_begin + r_1);
                ++r_1;
        }

        while (r_2 <= l_2) {
                tmp[i++] = *(m_begin + r_2);
                ++r_2;
        }

        auto[r, l] = full_range;
        assert((l - r + 1) == i && "temporary Range isn't equal to the original one\n");
        i = 0;
        while (r <= l) {
            *(m_begin + r) = tmp[i++];
            ++r;
        }
}

