#include <utility>
#include <iostream>
#include "ctime"
#include <fstream>
#include <sstream>
#include <vector>
#include <functional>
#include "data_structure.cpp"

using namespace std;
using namespace itis;

namespace parser {
  vector<int> split(const std::string &s, char delimiter) {
    vector<int> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
      tokens.push_back(stoi(token));
    }
    return tokens;
  }

  string pathToInputFile = "\\semester-work-b-tree\\src\\input.txt";

  string process_data() {
    ifstream file(pathToInputFile);
    string result;
    string line;

    while (getline(file, line)) {
      // create structure here
      auto *btree = new BTree(2);

      vector<int> intValues = split(line, ' ');    // splitting by delimiter and creating vector with int values
      result = to_string(intValues.size()) + ",";  // amount of elements. "," - delimiter
      double startTime = clock();
      // here is executing tests
      for (int value : intValues) {
        btree->insert(value);
      }
      double endTime = clock();
      cout << result << to_string(endTime - startTime) + "\n";  // output elements count and test complition time
    }
    file.close();
    return result;
  }

  int main() {
    cout << process_data();
    return 0;
  }
}  // namespace parser