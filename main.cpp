#include <iostream>
#include <parser.hpp>

using namespace std;

int main() {
  auto *parser = new Parser();
  string a = parser->process_data();
  cout << a;
  return 0;
}