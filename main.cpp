#include <iostream>
#include <parser.hpp>

#include "data_structure.hpp"

using namespace std;
using namespace itis;

int main() {
  auto *parser = new Parser();
  parser->process_data();
  return 0;
}