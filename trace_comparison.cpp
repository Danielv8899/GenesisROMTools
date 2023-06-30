#include <iostream>
#include <fstream>
#include <unordered_set>
#include <vector>
#include <string>

// Function to compare two trace files, remove repeating code, and mark unique code
void compareAndMarkUniqueCode(const std::string& file1, const std::string& file2) {
  std::unordered_set<std::string> uniqueCode;
  std::vector<std::string> uniqueCodeList;

  // Read the first trace file
  std::ifstream traceFile1(file1);
  if (!traceFile1.is_open()) {
    std::cerr << "Failed to open " << file1 << std::endl;
    return;
  }

  std::string line;
  while (std::getline(traceFile1, line)) {
    uniqueCode.insert(line);
  }

  traceFile1.close();

  // Read the second trace file
  std::ifstream traceFile2(file2);
  if (!traceFile2.is_open()) {
    std::cerr << "Failed to open " << file2 << std::endl;
    return;
  }

  while (std::getline(traceFile2, line)) {
    if (uniqueCode.find(line) == uniqueCode.end()) {
      uniqueCode.insert(line);
      uniqueCodeList.push_back(line);
    }
  }

  traceFile2.close();

  // Print the unique code
  std::cout << "Unique code:\n";
  for (const std::string& code : uniqueCodeList) {
    std::cout << code << '\n';
  }
}

int main() {
  std::string traceFile1 = "trace1.txt";
  std::string traceFile2 = "trace2.txt";

  compareAndMarkUniqueCode(traceFile1, traceFile2);

  return 0;
}