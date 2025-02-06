#include <stdexcept>
#include <fstream>
#include <string>
#include <regex>
#include <cassert>
#include <unistd.h>
#include "cpu.h"

namespace {

// core stats are in the format
//   cpuNN user nice system idle iowait irq softirq
const std::regex s_core_pat( "^cpu(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)" );

}

CPU::CPU()
  : m_ticks_per_sec(100) {

}

CPU::~CPU() {

}

// determine number of cores and ticks per second,
// and get initial stats
void CPU::init() {
  m_ticks_per_sec = sysconf( _SC_CLK_TCK );
  if ( m_ticks_per_sec < 0 )
    throw std::runtime_error( "sysconf failed" );
  poll();
}

// get number of cores
int CPU::get_num_cores() const {
  return int(m_cores.size());
}

// get number of ticks per second
int CPU::get_ticks_per_sec() const {
  return int(m_ticks_per_sec);
}

// this should be called once per second
// after calling init()
void CPU::poll() {
  std::ifstream proc_stat("/proc/stat");
  if ( !proc_stat.is_open() )
    throw std::runtime_error( "couldn't open /proc/stat" );
  
  std::string line;
  int n = 0;
  while ( std::getline( proc_stat, line ) ) {
    std::smatch m;
    if ( std::regex_search( line, m, s_core_pat ) ) {
      assert( m.size() == 9 );

      // add Core to vector if it's not there already
      if ( n >= int(m_cores.size()) )
        m_cores.push_back( Core() );

      // Core we are updating
      Core &core = m_cores[n];

      // Move forward in time
      core.last = core.now;

      // Parse stats
      uint64_t user    = std::stoul( m[2] ),
               nice    = std::stoul( m[3] ),
               sys     = std::stoul( m[4] ),
               idle    = std::stoul( m[5] ),
               iowait  = std::stoul( m[6] ),
               irq     = std::stoul( m[7] ),
               softirq = std::stoul( m[8] );
      (void) idle;
      
      // Set current stats for this Core
      core.now.user = user + nice;
      core.now.sys  = sys + iowait + irq + softirq;

      ++n;
    }
  }
}

// get user time consumed between last two polls (normalized in range 0.0-1.0)
double CPU::get_user_time( int core_index ) const {
  assert( core_index >= 0 );
  assert( core_index < get_num_cores() );
  const Core &core = m_cores[ core_index ];
  return elapsed( core.now.user, core.last.user );
}

// get system time consumed between last two polls (normalized in range 0.0-1.0)
double CPU::get_sys_time( int core_index ) const {
  assert( core_index >= 0 );
  assert( core_index < get_num_cores() );
  const Core &core = m_cores[ core_index ];
  return elapsed( core.now.sys, core.last.sys );
}

double CPU::elapsed( uint64_t nowval, uint64_t lastval ) const {
  uint64_t diff;
  if ( nowval < lastval )
    diff = 0; // shouldn't happen
  else 
    diff = nowval - lastval;
  return double(diff) / double(m_ticks_per_sec);
}
