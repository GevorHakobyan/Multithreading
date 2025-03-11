#pragma once
#include "Type_traits.hpp"
#include <vector>
#include <cassert>
#include <mutex>
#include <fstream>
#include <string>
#include <memory>
#include "Tool.hpp"

namespace task {
    template <typename T, typename Container = std::vector<T>>
    class File_Creator : public Tool<T, Container> {
        private:
            using Out_File = std::ofstream;
            using Mutex = std::mutex;
            using String = std::string;
            using Iterator = typename Tool<T, Container>::Iterator;
        public:
            constexpr Tool<T, Container>* clone() const override {return new File_Creator<T, Container>(*this);}
            constexpr File_Creator(const String& name);
            constexpr void operator()(Iterator begin, Iterator end) override;
        private:
            constexpr auto open_in_app_mode() const;
            constexpr bool create() const;
        private:
            String m_file_name{"Default"};
            static inline Mutex m_mutex;
    };
    #include "File_Creator.tpp"
}//namespace task