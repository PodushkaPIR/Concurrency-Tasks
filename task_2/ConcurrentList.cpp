#include "ConcurrentList.h"

#include <vector>
#include <mutex>
#include <algorithm>
#include <thread>
#include <stdexcept>
#include <iostream>

#include "Utils.h"

void ConcurrentList::generate_indices(std::vector<size_t>& indices) {
    // static std::atomic<unsigned> seed_counter = 0;
    // unsigned seed = static_cast<unsigned>(std::time(nullptr)) + seed_counter++;
    // std::srand(seed);
    //
    // indices.resize(m_modification_count);
    //
    // if (m_data.empty()) {
    //     indices.clear();
    //     return;
    // }
    //
    // for (size_t i = 0; i < m_modification_count; i++) {
    //     indices[i] = std::rand() % m_data.size();
    //
    //     bool duplicate_found = false;
    //     for (size_t j = 0; j < i; j++) {
    //         if (indices[j] == indices[i]) {
    //             duplicate_found = true;
    //             i--;
    //             break;
    //         }
    //     }
    // }
    static thread_local std::mt19937 gen(std::random_device{}());
    indices.clear();
    if (m_data.empty()) return;

    std::uniform_int_distribution<size_t> dist(0, m_data.size() - 1);
    std::set<size_t> unique_indices;

    while (unique_indices.size() < Utils::modification_count) {
        unique_indices.insert(dist(gen));
    }

    indices.assign(unique_indices.begin(), unique_indices.end());
}

void ConcurrentList::print() const {
    std::unique_lock lock(m_print_mutex);
    for(auto val : m_data) std::cout << val << " ";
    std::cout << "\n";
}

void NaiveList::transform() {
    std::vector<size_t> indices;
    generate_indices(indices);

    int sum = 0;
    for (auto idx : indices) {
        sum += m_data[idx];
    }

    for (auto idx : indices) {
        m_data[idx] = sum;
    }
}


void GlobalLockList::transform() {
    std::lock_guard<std::mutex> lock(m_global_mutex);

    std::vector<size_t> indices;
    generate_indices(indices);

    int sum = 0;
    for (auto idx : indices) {
        sum += m_data[idx];
    }

    for (auto idx : indices) {
        m_data[idx] = sum;
    }
}

bool RollbackLockList::try_full_locks(const std::vector<size_t>& indices, std::vector<std::unique_lock<std::mutex>>& locks) {
    for (auto idx : indices) {
        locks.emplace_back(m_element_mutexes[idx], std::try_to_lock);
        if (!locks.back().owns_lock()) return false;
    }
    return true;
}

void RollbackLockList::transform() {
    std::vector<size_t> indices;
    generate_indices(indices);

    int attempts = 0;

    while (attempts++ < Utils::max_attempts) {
        std::vector<std::unique_lock<std::mutex>> locks;
        locks.reserve(indices.size());

        if (try_full_locks(indices, locks)) {
            int sum = 0;
            for (auto idx : indices) sum += m_data[idx];
            for (auto idx : indices) m_data[idx] = sum;
            return;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
    throw std::runtime_error("Failed to take locks after 100 attempts");
}

void OrderedLockList::transform() {
    std::vector<size_t> indices;
    generate_indices(indices);
    std::sort(indices.begin(), indices.end());

    std::vector<std::unique_lock<std::mutex>> locks;
    locks.reserve(indices.size());

    for (auto idx : indices) {
        locks.emplace_back(m_element_mutexes[idx]);
    }

    int sum = 0;
    for (auto idx : indices) {
        sum += m_data[idx];
    }
    for (auto idx : indices) {
        m_data[idx] = sum;
    }
}

