#pragma once
#include "Type_traits.hpp"
#include <vector>
#include <cassert>
#include "Tool.hpp"

namespace task {
    template <typename T, typename Container>
    class Merge_Sort : public Tool<T, Container> {
        static_assert(traits::has_begin_end<Container>::value, "Container must support begin and end\n");
        static_assert(traits::is_comparable<T>::value,"Elements must be comparable\n");
        //usings
        using Range = std::pair<size_t, size_t>;
        using Iterator = typename Tool<T, Container>::Iterator;
        static_assert(traits::is_random_access_iterator<Iterator>::value, "Provided argument must be random access iterator\n");  
        public:
            constexpr Tool<T, Container>* clone() const override {return new Merge_Sort<T, Container>(*this);}
            constexpr void operator()(Iterator begin, Iterator end) override;
            constexpr void set_begin_end(Iterator begin, Iterator end);
            constexpr void set_size(Iterator begin, Iterator end);
        private: //helper methods
            constexpr  void divide_comp_merge(size_t first, size_t second);
            constexpr  void merge(Range right, Range left); 
        private:
            Iterator m_begin{Iterator()};
            Iterator m_end{Iterator()};
            size_t m_size{0};
    };
    #include "Merge_Sort.tpp"
}//namespace task