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

std::string Parser::process_data(const string &pathToInputFile) {
  ifstream file(pathToInputFile);
  string result;
  string line;
  vector<float> insert;
  vector<float> search;
  vector<float> searchKey;
  vector<float> remove;

  while (getline(file, line)) {
    // create structure here
    auto *btree = new BTree(2);

    vector<int> intValues = split(line, ' ');
    result = to_string(intValues.size()) + " elements";
    cout << result << "\n";
    //insert
    double startTime = clock();
    for (int value : intValues) {
      btree->insert(value);
    }
    double endTime = clock();
    insert.emplace_back(endTime - startTime);
    cout << "insert :" << to_string(endTime - startTime) + " ms\n";

    //search
    startTime = clock();
    for (int value : intValues) {
      btree->search(value);
    }
    endTime = clock();
    search.emplace_back(endTime - startTime);
    cout << "search :" << to_string(endTime - startTime) + " ms\n";

    //searchKey
    startTime = clock();
    for (int value : intValues) {
      btree->searchKey(value);
    }
    endTime = clock();
    searchKey.emplace_back(endTime - startTime);
    cout << "searchKey :" << to_string(endTime - startTime) + " ms\n";

    //remove
    startTime = clock();
    for (int value : intValues) {
      btree->remove(value);
    }
    endTime = clock();
    remove.emplace_back(endTime - startTime);
    cout << "remove :" << to_string(endTime - startTime) + " ms\n";
  }
  file.close();

  float average = 0;
  for (float f : insert) {
    average += f;
  }
  average /= static_cast<float>(insert.size());
  cout << "\nAverage insert: " << average << " ms\n";

  average = 0;
  for (float f : search) {
    average += f;
  }
  average /= static_cast<float>(search.size());
  cout << "Average search: " << average << " ms\n";

  average = 0;
  for (float f : searchKey) {
    average += f;
  }
  average /= static_cast<float>(searchKey.size());
  cout << "Average searchKey: " << average << " ms\n";

  average = 0;
  for (float f : remove) {
    average += f;
  }
  average /= static_cast<float>(remove.size());
  cout << "Average remove: " << average << " ms\n";

  return result;
}
