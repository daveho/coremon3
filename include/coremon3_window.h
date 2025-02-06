#ifndef COREMON3_WINDOW_H
#define COREMON3_WINDOW_H

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include "cpu.h"

class Coremon3Window : public Fl_Window {
private:
  CPU m_cpu;
  int m_mouse_x, m_mouse_y;

  static const Fl_Color BACKGROUND = 0x00002000;  // very dark blue
  static const Fl_Color USER = 0x1e90ff00;        // dodger blue
  static const Fl_Color SYS = 0x19197000;         // midnight blue

  static const int POLLS_PER_SEC = 4;

public:
  Coremon3Window();
  ~Coremon3Window();

  void draw() override;
  void show( int argc, char **argv );

  int handle( int event ) override;

private:
  static void on_timer_tick( void *data );
  void set_always_on_top();
};

#endif // COREMON3_WINDOW_H
