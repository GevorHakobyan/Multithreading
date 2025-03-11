#pragma once
#include <vector>
#include <memory>

namespace task
{
   template <typename T, typename Container = std::vector<T>>
   class Tool {
    protected:
        using Iterator = Container::iterator;
        using this_ptr = Tool*;
    public:
        constexpr virtual void operator() (Iterator begin, Iterator end) = 0;
        constexpr virtual Tool<T, Container>* clone() const = 0;
        constexpr virtual ~Tool() = default;
   };
} // namespace task