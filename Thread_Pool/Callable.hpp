#pragma once
#include "Tool.hpp"

namespace task
{
    template <typename T, typename Container>
    class Collable {
        private:
            using tool_ptr = std::shared_ptr<Tool<T, Container>>;
            using Iterator = Container::iterator;
            using raw_tool = Tool<T, Container>*;
        public:
           Collable() = default;
           Collable(const Collable& rhs) {m_tool.reset(rhs.m_tool->clone());}
            void reset_tool(raw_tool ptr) {m_tool.reset(ptr);}
            void operator() (Iterator begin, Iterator end) const{
                assert(m_tool != nullptr);
                m_tool->operator()(begin, end);
            }
        private:
            tool_ptr m_tool{nullptr};
    };
} // namespace task
