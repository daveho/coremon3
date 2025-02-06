#include <iostream>
#include <unistd.h>
#include "coremon3_window.h"
#include "cpu.h"

int main( int argc, char **argv ) {
#if 0
  CPU cpu;

  cpu.init();
  int num_cores = cpu.get_num_cores();
  std::cout << num_cores << " core(s)\n";
  for ( int i = 0; i < 5; ++i ) {
    sleep( 1 );
    cpu.poll();
    for ( int j = 0; j < num_cores; ++j )
      std::cout << "user=" << cpu.get_user_time( j ) << ", sys=" << cpu.get_sys_time( j ) << "\n";
  }

  return 0;
#endif

  auto win = Coremon3Window();
  win.show( argc, argv );
  return Fl::run();
}
