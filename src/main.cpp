#include <iostream>
#include <unistd.h>
#include "coremon3_window.h"
#include "cpu.h"

int main( int argc, char **argv ) {
  auto win = Coremon3Window();
  win.show( argc, argv );
  return Fl::run();
}
