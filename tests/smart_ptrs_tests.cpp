#include "smart_ptrs_tests.h"
#include "../src/smart_ptrs.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>

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
        int* rawPtr = p1.release();
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
        for (int i = 0; i < 1000000; ++i) {
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
        result << (*p2 == 20 ? "Passed" : "Failed") << "\n";
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
