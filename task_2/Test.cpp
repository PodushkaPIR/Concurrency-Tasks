#include "Test.h"

#include <chrono>
#include <iostream>
#include <vector>
#include <thread>

#include "ConcurrentList.h"
#include "Utils.h"

void benchmark_list(ConcurrentList* list, int thread_count, const std::string& test_name) {
    const std::vector<int> original_data = list->data_copy();
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<std::thread> threads;
    int ops = Utils::operations_per_thread;
    for(int i = 0; i < thread_count; ++i) {
        threads.emplace_back([&]() {
            try {
                for(int j = 0; j < ops; ++j) {
                    list->transform();
                }
            } catch (const std::exception& e) {
                std::cerr << "Error in thread: " << e.what() << "\n";
            }
        });
    }
    
    for(auto& t : threads) {
        t.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    const std::vector<int> result_data = list->data_copy();
    
    // 5. Вывод результатов
    std::cout << "\n-----------------------------------------\n";
    std::cout << "[" << test_name << "]\n";
    std::cout << "Threads: " << thread_count << "\n";
    
    if(original_data.size() <= 20) {
        std::cout << "Original data: ";
        for(auto val : original_data) std::cout << val << " ";
        std::cout << "\nResult data:   ";
        for(auto val : result_data) std::cout << val << " ";
        std::cout << "\n";
    }
    
    std::cout << "Execution time: " << duration.count() << " mks\n";
    std::cout << "-----------------------------------------\n";
}

void test_naive_list(std::vector<int> data, int threads) {
    NaiveList list(data);
    benchmark_list(&list, threads, "Naive List (unsafe)");
}

void test_global_lock(std::vector<int> data, int threads) {
    GlobalLockList list(data);
    benchmark_list(&list, threads, "Global Lock List");
}

void test_rollback_lock(std::vector<int> data, int threads) {
    RollbackLockList list(data);
    benchmark_list(&list, threads, "Rollback Lock List");
}

void test_ordered_lock(std::vector<int> data, int threads) {
    OrderedLockList list(data);
    benchmark_list(&list, threads, "Ordered Lock List");
}

void run_all_tests() {
    const std::vector<int> test_data = {1, 3, 5, 6, 7, 9, 10, 11};
    const int thread_count = 8;
    
    test_naive_list(test_data, thread_count);
    // test_global_lock(test_data, thread_count);
    // test_rollback_lock(test_data, thread_count);
    // test_ordered_lock(test_data, thread_count);

    // const int large_size = 1000;
    // std::vector<int> large_data(large_size, 0);
    // std::cout << "\n\n===== LARGE DATA TEST (size: " << large_size << ") =====\n";
    // 
    // test_global_lock(large_data, thread_count);
    // test_rollback_lock(large_data, thread_count);
    // test_ordered_lock(large_data, thread_count);
}
