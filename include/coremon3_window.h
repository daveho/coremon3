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
