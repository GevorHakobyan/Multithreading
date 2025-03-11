#include "File_Creator.hpp"


template <typename T, typename Container>
constexpr task::File_Creator<T, Container>::File_Creator(const String& name) 
:Tool<T, Container>{}, m_file_name{name} 
{
    assert(create() && "Error: Unable to create a file\n");
};


template <typename T, typename Container>
constexpr void task::File_Creator<T, Container>::operator() (Iterator begin, Iterator end) {
    assert(begin <= end && "Invalid range provided\n");

    std::lock_guard<std::mutex> lock(m_mutex);
    //open in app mode
    auto f_ptr = open_in_app_mode(); 
    if (begin == end) {
        *f_ptr << *begin << '\n';
        return;
    }
    while (begin != end) {
        *f_ptr << *begin << '\n';
        ++begin;
    }
}

template <typename T, typename Container>
constexpr bool task::File_Creator<T, Container>::create() const {
    Out_File m_file{m_file_name};
    if (!m_file.is_open()) {
        std::cerr << "Error: Couldn't open the file\n";
        return false;
    }
    m_file.close();
    return true;
}

template <typename T, typename Container>
constexpr auto task::File_Creator<T, Container>::open_in_app_mode() const ->auto {
    auto u_ptr = std::make_unique<Out_File>(m_file_name, std::ios::app);
    if (!u_ptr->is_open()) {
        std::cerr << "Error: Couldn't open the file for reading" << std::endl;
    }
    return u_ptr;
}