#include "smart_ptrs_tests.h"
#include "../src/smart_ptrs.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include "test_structure.h"
#include "memory"
std::string runUnqPtrTests() {
    std::ostringstream result;
    result << "UnqPtr Tests:\n";

    result << "  Functional Test 1: ";
    {
        UnqPtr<int> p1(new int(10));
        result << (*p1 == 10 ? "Passed" : "Failed") << "\n";
    }

    result << "  Functional Test 2: ";
    {
        UnqPtr<int> p1(new int(20));
        int *rawPtr = p1.release();
        result << (*rawPtr == 20 ? "Passed" : "Failed") << "\n";
        delete rawPtr;
    }

    result << "  Load Test 1 (Small): ";
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<UnqPtr<int>> pointers;
        for (int i = 0; i < 1000; ++i) {
            pointers.push_back(UnqPtr<int>(new int(i)));
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        result << "Time: " << duration << " ms\n";
    }

    result << "  Load Test 2 (Large): ";
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<UnqPtr<int>> pointers;
        for (int i = 0; i < 10'000'000; ++i) {
            pointers.push_back(UnqPtr<int>(new int(i)));
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        result << "Time: " << duration << " ms\n";
    }

    return result.str();
}


std::string runShrdPtrTests() {
    std::ostringstream result;
    result << "ShrdPtr Tests:\n";

    result << "  Functional Test 1: ";
    {
        UnqPtr<int> uptr(new int(10));
        ShrdPtr<int> p1(&uptr);
        result << (*p1 == 10 ? "Passed" : "Failed") << "\n";
    }

    result << "  Functional Test 2: ";
    {
        UnqPtr<int> uptr(new int(20));
        ShrdPtr<int> p1(&uptr);
        ShrdPtr<int> p2 = std::move(p1);
        result << (*p2 == 20 && *p1 == 20 ? "Passed" : "Failed") << "\n";
    }

    result << "  Load Test 1 (Small): ";
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<ShrdPtr<int>> pointers;
        for (int i = 0; i < 1000; ++i) {
            UnqPtr<int> uptr(new int(i));
            pointers.push_back(ShrdPtr<int>(&uptr));
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        result << "Time: " << duration << " ms\n";
    }

    result << "  Load Test 2 (Large): ";
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<ShrdPtr<int>> pointers;
        for (int i = 0; i < 1000000; ++i) {
            UnqPtr<int> uptr(new int(i));
            pointers.push_back(ShrdPtr<int>(&uptr));
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        result << "Time: " << duration << " ms\n";
    }

    return result.str();
}


std::string runLinkedListUniqueTests() {
    std::ostringstream result;
    result << "LinkedList Tests:\n";

    result << "  Functional Test 1: ";
    {
        LinkedList<int> list;
        list.push_front(10);
        result << (list.size() == 1 ? "Passed" : "Failed") << "\n";
    }

    result << "  Functional Test 2: ";
    {
        LinkedList<int> list;
        list.push_front(10);
        list.pop_front();
        result << (list.size() == 0 ? "Passed" : "Failed") << "\n";
    }

    result << "  Functional Test 3: ";
    {
        LinkedList<int> list;
        result << (list.null() ? "Passed" : "Failed") << "\n";
    }

    result << "  Functional Test 4: ";
    {
        LinkedList<int> list;
        list.push_front(10);
        list.push_front(20);
        list.clear();
        result << (list.size() == 0 && list.null() ? "Passed" : "Failed") << "\n";
    }

    result << "  Load Test 1 (Small): ";
    {
        auto start = std::chrono::high_resolution_clock::now();
        LinkedList<int> list;
        for (int i = 0; i < 1000; ++i) {
            list.push_front(i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        result << "Time: " << duration << " ms, Size: " << list.size() << "\n";
    }

    result << "  Load Test 2 (Large): ";
    {
        auto start = std::chrono::high_resolution_clock::now();
        LinkedList<int> list;
        for (int i = 0; i < 1'000'000; ++i) {
            list.push_front(i);
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        result << "Time: " << duration << " ms, Size: " << list.size() << "\n";
    }

    return result.str();
}

std::string runStdUnqPtrTests() {
    std::ostringstream result;
    result << "std::unique_ptr Tests:\n";

    result << "  Functional Test 1: ";
    {
        std::unique_ptr<int> p1(new int(10));
        result << (*p1 == 10 ? "Passed" : "Failed") << "\n";
    }

    result << "  Functional Test 2: ";
    {
        std::unique_ptr<int> p1(new int(20));
        int *rawPtr = p1.release();
        result << (*rawPtr == 20 ? "Passed" : "Failed") << "\n";
        delete rawPtr;
    }

    result << "  Load Test 1 (Small): ";
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<std::unique_ptr<int>> pointers;
        for (int i = 0; i < 1000; ++i) {
            pointers.push_back(std::unique_ptr<int>(new int(i)));
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        result << "Time: " << duration << " ms\n";
    }

    result << "  Load Test 2 (Large): ";
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<std::unique_ptr<int>> pointers;
        for (int i = 0; i < 10'000'000; ++i) {
            pointers.push_back(std::unique_ptr<int>(new int(i)));
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        result << "Time: " << duration << " ms\n";
    }

    return result.str();
}

std::string runStdShrdPtrTests() {
    std::ostringstream result;
    result << "std::shared_ptr Tests:\n";

    result << "  Functional Test 1: ";
    {
        std::shared_ptr<int> p1 = std::make_shared<int>(10);
        result << (*p1 == 10 ? "Passed" : "Failed") << "\n";
    }

    result << "  Functional Test 2: ";
    {
        std::shared_ptr<int> p1 = std::make_shared<int>(20);
        std::shared_ptr<int> p2 = p1;
        result << (*p2 == 20 ? "Passed" : "Failed") << "\n";
    }

    result << "  Load Test 1 (Small): ";
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<std::shared_ptr<int>> pointers;
        for (int i = 0; i < 1000; ++i) {
            pointers.push_back(std::make_shared<int>(i));
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        result << "Time: " << duration << " ms\n";
    }

    result << "  Load Test 2 (Large): ";
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::vector<std::shared_ptr<int>> pointers;
        for (int i = 0; i < 1'000'000; ++i) {
            pointers.push_back(std::make_shared<int>(i));
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        result << "Time: " << duration << " ms\n";
    }

    return result.str();
}

//for charts(для графиков)

std::vector<std::pair<int, long long>> loadUnqPtrTests() {
    std::vector<std::pair<int, long long>> result;
    std::vector<int> test_sizes = {10'000, 100'000, 1'000'000, 10'000'000};
    for(int test_size: test_sizes) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < test_size; ++i) {
            UnqPtr<int>(new int(i));
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        result.push_back({test_size, duration});
    }

    return result;
}


std::vector<std::pair<int, long long>> loadShrdPtrTests() {
    std::vector<std::pair<int, long long>> result;
    std::vector<int> test_sizes = {10'000, 100'000, 1'000'000, 10'000'000};
    for(int test_size: test_sizes) {
        auto start = std::chrono::high_resolution_clock::now();
        {
            std::vector<ShrdPtr<int>> pointers;
            UnqPtr<int> uptr(new int(3));
            ShrdPtr<int> sptr(&uptr);
            for (int i = 0; i < test_size; ++i) {
                pointers.push_back(ShrdPtr<int>(sptr));
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        result.push_back({test_size, duration});
    }
    return result;
}

std::vector<std::pair<int, long long>> loadStdUnqPtrTests() {
    std::vector<std::pair<int, long long>> result;
    std::vector<int> test_sizes = {10'000, 100'000, 1'000'000, 10'000'000};
    for(int test_size: test_sizes){
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < test_size; ++i) {
            std::unique_ptr<int>(new int(i));
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        result.push_back({test_size, duration});
    }

    return result;
}

std::vector<std::pair<int, long long>> loadStdShrdPtrTests() {
    std::vector<std::pair<int, long long>> result;
    std::vector<int> test_sizes = {10'000, 100'000, 1'000'000, 10'000'000};
    auto start = std::chrono::high_resolution_clock::now();
    for(int test_size: test_sizes) {
        {
            std::vector<std::shared_ptr<int>> pointers;
            for (int i = 0; i < test_size; ++i) {
                pointers.push_back(std::make_shared<int>(i));
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        result.push_back({test_size, duration});
    }
    return result;
}