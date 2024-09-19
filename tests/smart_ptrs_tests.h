#pragma once
#include <string>
#include "vector"
std::string runUnqPtrTests();
std::string runShrdPtrTests();
std::string runLinkedListUniqueTests();
std::string runStdUnqPtrTests();
std::string runStdShrdPtrTests();

std::vector<std::pair<int, long long>> loadUnqPtrTests();
std::vector<std::pair<int, long long>> loadShrdPtrTests();
std::vector<std::pair<int, long long>> loadStdShrdPtrTests();
std::vector<std::pair<int, long long>> loadStdUnqPtrTests();

