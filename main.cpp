#include <iostream>
#include <parser.hpp>

using namespace std;

int main() {
  cout << "Time test:\n";
  string path = "C:\\Users\\007\\Desktop\\Vigvamcev\\semester-work-b-tree\\tests\\1000000.csv";
  string a = Parser::process_data(path);
  return 0;
}