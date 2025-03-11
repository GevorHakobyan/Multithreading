#include <vector>
#include <list>
#include <iostream>
#include <deque>
#include "Thread_Pool.hpp"
#include "File_Creator.hpp"
#include <numeric>
#include <algorithm>
#include "Tool.hpp"


int main() {
    //prepare everything
    std::vector<int> vec(155);
    std::iota(vec.begin(), vec.end(), 1);
    std::reverse(vec.begin(), vec.end());
    //prepare thread pool for merge sort
    threads::Thread_Pool<int> pool(vec.begin(), vec.end(), new task::Merge_Sort<int,std::vector<int>>());

    auto start = std::chrono::high_resolution_clock::now();
    //sort
    pool.action();  

    //write in a file
    task::File_Creator<int>* ptr = new task::File_Creator<int>("File.txt");
    pool.change_tool(vec.begin(), vec.end(), ptr, threads::Thread_Pool<int>::Tool_Type::Linear);
    pool.action();
    
    auto end = std::chrono::high_resolution_clock::now();
    
    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time: " << duration.count() << " seconds\n";
    return 0;
}

