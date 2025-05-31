#pragma once

#include <string>

#include "ConcurrentList.h"

void benchmark_list(ConcurrentList* list, int thread_count, const std::string& test_name);

void test_naive_list(std::vector<int> data, int threads);
void test_global_lock(std::vector<int> data, int threads);
void test_rollback_lock(std::vector<int> data, int threads);
void test_ordered_lock(std::vector<int> data, int threads);

void run_all_tests();
