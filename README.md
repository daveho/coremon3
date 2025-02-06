# coremon3: very simple CPU core monitor

This program is an incredibly simple Linux CPU core monitor program.
Its intended purpose is to monitor CPU core activity by reading
`/proc/stat` at regular intervals (currently hard-coded at 4 times
per second) and then drawing a bar graph showing user and system time
for each core.

It's implemented using [fltk](https://www.fltk.org/). On Ubuntu 24.04
and similar, you can install it using the command

```bash
sudo apt install libfltk1.3-dev
```

To build `coremon3`:

```bash
make depend
make
```

The executable is `build/coremon3`.

## Invoking

The program supports all of the standard command line arguments
processed by [Fl_Window::show](https://www.fltk.org/doc-1.3/classFl__Window.html#a0d75dfcdc8fb797f9247482da1e0152b),
including `-geometry` to set the windows initial size and position.

The following `coremon3`-specific command line arguments are supported:

* `--always-on-top`: the `coremon3` window has the "always on top"
  window manager flag set, so it's always shown above other windows
* `--no-border`: don't show Window manager decorations: note that this
  means that there will be no task bar entry for `coremon3`

To make the bar graph fill the width of the window, the window width
should be an integer multiple of the number of CPU cores. On my six-core,
twelve-thread Ryzen 5 5600X, I invoke `coremon3` as

```bash
$SOME_PATH/coremon3 --always-on-top --no-border -geometry 120x60-200+0
```

which positions the window near the upper-right of the display.

## Quitting

Typing "q" or "Q" while the mouse pointer is inside the `coremon3`
window will exit the program.