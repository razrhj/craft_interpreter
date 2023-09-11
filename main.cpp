#include "./Scanning.hpp"

int main (int argc, char *argv[]) {
  Scanning sc;
  sc.readFromFile("./json_examples.json");
  return 0;
}
