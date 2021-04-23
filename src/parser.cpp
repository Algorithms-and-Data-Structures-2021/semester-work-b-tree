#include <utility>
#include <iostream>
#include "ctime"
#include <fstream>
#include <sstream>
#include <vector>
#include <functional>
#include <parser.hpp>

#include "data_structure.cpp"

using namespace std;
using namespace itis;

vector<int> split(const std::string &s, char delimiter) {
  vector<int> tokens;
  string token;
  istringstream tokenStream(s);
  while (getline(tokenStream, token, delimiter)) {
    tokens.push_back(stoi(token));
  }
  return tokens;
}

string pathToInputFile = "src\\input.txt";

std::string Parser::process_data() {
  ifstream file(pathToInputFile);
  string result;
  string line;

  while (getline(file, line)) {
    // create structure here
    auto *btree = new BTree(2);

    vector<int> intValues = split(line, ' ');
    result = to_string(intValues.size()) + ",";
    double startTime = clock();
    // here is executing tests
    for (int value : intValues) {
      btree->insert(value);
    }
    double endTime = clock();
    cout << result << to_string(endTime - startTime) + "\n";
  }
  file.close();
  return result;
}
