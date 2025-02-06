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

  // get user time in last second (normalized in range 0.0-1.0)
  double get_user_time( int core_index ) const;

  // get system time in last second (normalized in range 0.0-1.0)
  double get_sys_time( int core_index ) const;

private:
  double elapsed( uint64_t nowval, uint64_t lastval ) const;
};

#endif // CPU_H
