#include <cmath>
#include "coremon3_window.h"

Coremon3Window::Coremon3Window()
  : Fl_Window( 120, 60, "coremon3" ) {
  m_cpu.init();
  color( BACKGROUND );
  Fl::add_timeout( 1.0/POLLS_PER_SEC, on_timer_tick, static_cast<void*>( this ) );
}

Coremon3Window::~Coremon3Window() {

}

void Coremon3Window::draw() {
  Fl_Window::draw();

  // determine number of cores and expected ticks per poll
  int num_cores = m_cpu.get_num_cores();
  int ticks_per_sec = m_cpu.get_ticks_per_sec();
  double ticks_per_poll = double(ticks_per_sec) / POLLS_PER_SEC;

  // determine how large the bars will be
  int bar_height_max = h();
  int bar_width = w() / num_cores;

  // draw the bars based on user and system time for each core
  for ( int i = 0; i < num_cores; ++i ) {
    int user_ticks = m_cpu.get_user_ticks( i );
    int sys_ticks = m_cpu.get_sys_ticks( i );

    int user_height = int( ::round( ( double(user_ticks) / ticks_per_poll ) * bar_height_max ) );
    int sys_height = int( ::round( ( double(sys_ticks) / ticks_per_poll ) * bar_height_max ) );
    if ( (user_height + sys_height) > bar_height_max )
      --sys_height; // just in case both user and sys height got rounded up

    int x = i*bar_width;
    
    fl_draw_box( FL_FLAT_BOX, x, bar_height_max - user_height, bar_width, user_height, USER );
    fl_draw_box( FL_FLAT_BOX, x, bar_height_max - user_height - sys_height, bar_width, sys_height, SYS );
  }
}

void Coremon3Window::on_timer_tick( void *data ) {
  Coremon3Window *w = reinterpret_cast< Coremon3Window* >( data );
  w->m_cpu.poll();
  w->redraw();
  Fl::repeat_timeout( 1.0/POLLS_PER_SEC, on_timer_tick, data );
}