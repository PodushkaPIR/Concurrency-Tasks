#pragma once

#include <vector>
#include <mutex>
#include <random>
#include <algorithm>
#include <set>
#include <thread>
#include <stdexcept>
#include <iostream>
#include <mutex>
#include <shared_mutex>

class ConcurrentList {
protected:
    std::vector<int> m_data;
    mutable std::shared_mutex m_print_mutex;

    virtual void generate_indices(std::vector<size_t>& indices);

public:
    ConcurrentList(std::vector<int> data) : m_data(std::move(data)) {}

    const std::vector<int>& data() const { std::shared_lock lock(m_print_mutex); return m_data; }
    std::vector<int> data_copy() const { std::shared_lock lock(m_print_mutex); return m_data; }

    virtual ~ConcurrentList() = default;
    virtual void transform() = 0;

    void print() const;
};

class NaiveList : public ConcurrentList {
public:
    NaiveList(std::vector<int> data) : ConcurrentList(std::move(data)) {}
    void transform() override;
};

// task A
class GlobalLockList : public ConcurrentList {
    std::mutex m_global_mutex;
    
public:
    GlobalLockList(std::vector<int> data) : ConcurrentList(std::move(data)) {}
    void transform() override;
};

// task B
class RollbackLockList : public ConcurrentList {
    std::vector<std::mutex> m_element_mutexes;
    
    bool try_full_locks(const std::vector<size_t>& indices, std::vector<std::unique_lock<std::mutex>>& locks);
    
public:
    RollbackLockList(std::vector<int> data)
        : ConcurrentList(std::move(data)), m_element_mutexes(m_data.size()) {}
    void transform() override;
};

// task D
class OrderedLockList : public ConcurrentList {
    std::vector<std::mutex> m_element_mutexes;
    
public:
    OrderedLockList(std::vector<int> data)
        : ConcurrentList(std::move(data)), m_element_mutexes(m_data.size()) {}
    void transform() override;
};
