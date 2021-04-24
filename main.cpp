#include <iostream>
#include <parser.hpp>

using namespace std;

int main() {
  string path;
  cout << "Enter path to file: ";
  cin >> path;
  cout << "Time test:\n";
  string a = Parser::process_data(path);
  return 0;
}