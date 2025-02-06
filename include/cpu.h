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

  // this should be called once per second
  // after calling init()
  void poll();

  // get number of user ticks since last poll()
  int get_user_ticks( int core_index ) const;

  // get number of system ticks since last poll()
  int get_sys_ticks( int core_index ) const;
};

#endif // CPU_H
