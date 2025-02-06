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

#include <cmath>
#include <string>
#include "coremon3_window.h"

Coremon3Window::Coremon3Window()
  : Fl_Window( 120, 60, "coremon3" )
  , m_mouse_x( 0 )
  , m_mouse_y( 0 ) {
  m_cpu.init();
  color( BACKGROUND );
  Fl::add_timeout( 1.0/POLLS_PER_SEC, on_timer_tick, static_cast<void*>( this ) );
}

Coremon3Window::~Coremon3Window() {

}

void Coremon3Window::draw() {
  // paint background
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

void Coremon3Window::show( int argc, char **argv ) {
  // Handle coremon3-specific command line arguments.
  // All other arguments are collected so we can pass them to
  // Fl_Window::show().

  // command line arguments to pass through to fltkk
  std::vector<char *> fltk_args;

  // argv[0] stays the same
  fltk_args.push_back( argv[0] );

  bool always_on_top = false;
  bool no_border = false;

  for ( int i = 1; i < argc; ++i ) {
    std::string arg = argv[i];

    if ( arg == "--always-on-top" )
      always_on_top = true;
    else if ( arg == "--no-border" )
      no_border = true;
    else
      fltk_args.push_back( argv[i] );
  }
  fltk_args.push_back( nullptr );

  if ( no_border )
    border( 0 );

  // handle fltk options (and actually show the window)
  Fl_Window::show( int( fltk_args.size() - 1 ), fltk_args.data() );

  if ( always_on_top )
    set_always_on_top();
}

int Coremon3Window::handle( int event ) {
  // The idea here is to allow the window to be
  // moved by dragging anywhere in its visible area.
  // We also support exiting the program if 'q' or 'Q'
  // is pressed.
  switch ( event ) {
  case FL_PUSH:
    if ( Fl::event_button1() ) {
      m_mouse_x = Fl::event_x();
      m_mouse_y = Fl::event_y();
      return 1;
    } else
      return 0;
  case FL_DRAG:
    {
      int dx = Fl::event_x() - m_mouse_x;
      int dy = Fl::event_y() - m_mouse_y;
      int updated_x = x() + dx;
      int updated_y = y() + dy;
      resize( updated_x, updated_y, w(), h() );
      return 1;
    }
    case FL_KEYDOWN:
      {
        std::string key = Fl::event_text();
        if ( key == "q" || key == "Q" )
          hide();
      }
      break;
  }

  return 0;
}

void Coremon3Window::on_timer_tick( void *data ) {
  Coremon3Window *w = reinterpret_cast< Coremon3Window* >( data );
  w->m_cpu.poll();
  w->redraw();
  Fl::repeat_timeout( 1.0/POLLS_PER_SEC, on_timer_tick, data );
}

void Coremon3Window::set_always_on_top() {
  // See: https://groups.google.com/g/fltkgeneral/c/Je0bmtom-Og
  XEvent ev;
  static const char* const names[2] = { "_NET_WM_STATE",
                                        "_NET_WM_STATE_ABOVE"
                                      };
  Atom atoms[ 2 ];
  fl_open_display();
  XInternAtoms(fl_display, (char**)names, 2, False, atoms );
  Atom net_wm_state = atoms[ 0 ];
  Atom net_wm_state_above = atoms[ 1 ];
  ev.type = ClientMessage;
  ev.xclient.window = fl_xid(this);
  ev.xclient.message_type = net_wm_state;
  ev.xclient.format = 32;
  ev.xclient.data.l[ 0 ] = (int) true;
  ev.xclient.data.l[ 1 ] = net_wm_state_above;
  ev.xclient.data.l[ 2 ] = 0;
  XSendEvent(fl_display,
              DefaultRootWindow(fl_display),  False,
              SubstructureNotifyMask|SubstructureRedirectMask, &ev);
}
