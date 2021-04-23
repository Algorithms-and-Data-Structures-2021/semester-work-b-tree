#include <utility>
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

string pathToInputFile = "C:\\Users\\007\\Desktop\\Vigvamcev\\semester-work-b-tree\\src\\input.txt";

std::string Parser::process_data() {
  ifstream file(pathToInputFile);
  string result;
  string line;

  while (getline(file, line)) {
    // create structure here
    auto *btree = new BTree(2);

    vector<int> intValues = split(line, ' ');
    result = to_string(intValues.size()) + " elements";

    //insert
    double startTime = clock();
    for (int value : intValues) {
      btree->insert(value);
    }
    double endTime = clock();
    cout << "insert :" << to_string(endTime - startTime) + " ms\n";

    //search
    startTime = clock();
    for (int value : intValues) {
      btree->search(value);
    }
    endTime = clock();
    cout << "search :" << to_string(endTime - startTime) + " ms\n";

    //searchKey
    startTime = clock();
    for (int value : intValues) {
      btree->searchKey(value);
    }
    endTime = clock();
    cout << "searchKey :" << to_string(endTime - startTime) + " ms\n";

    //remove
    startTime = clock();
    for (int value : intValues) {
      btree->remove(value);
    }
    endTime = clock();
    cout << "remove :" << to_string(endTime - startTime) + " ms\n";
  }
  file.close();
  return result;
}
