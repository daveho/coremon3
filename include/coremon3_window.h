#ifndef COREMON3_WINDOW_H
#define COREMON3_WINDOW_H

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include "cpu.h"

class Coremon3Window : public Fl_Window {
private:
  CPU m_cpu;

public:
  Coremon3Window();
  ~Coremon3Window();

  void draw() override;
};

#endif // COREMON3_WINDOW_H
