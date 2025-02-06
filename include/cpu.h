// Copyright (c) 2025, David H. Hovemeyer <david.hovemeyer@gmail.com>
// 
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// “Software”), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
// NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
// THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef CPU_H
#define CPU_H

#include <vector>
#include <cstdint>

class CPU {
public:
  // most recent user and system time observed
  // for one core
  struct State {
    uint64_t user=0, sys=0;
  };
  struct Core {
    State last, now;
  };

private:
  std::vector<Core> m_cores;
  long m_ticks_per_sec;

public:
  CPU();
  ~CPU();

  // determine number of cores and ticks per second,
  // and get initial stats
  void init();

  // get number of cores
  int get_num_cores() const;

  // get number of ticks per second
  int get_ticks_per_sec() const;

  // get updated CPU utilization from /proc/stat
  void poll();

  // get number of user ticks since last poll()
  int get_user_ticks( int core_index ) const;

  // get number of system ticks since last poll()
  int get_sys_ticks( int core_index ) const;
};

#endif // CPU_H
