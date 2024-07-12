#include <iostream>
#include <sandpile.h>
#include <argument_parser.h>

int main(int argc, char** argv) {
  Sandpile my;
  Parser parser;

  parser.Parse(argc, argv, my);
  my.SetInitialState();
  my.RunApp();
}
