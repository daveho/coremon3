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

Planned features (not implemented yet):

* Command line argument to set window size
* Command line argument to enable "always on top"
* Supress window manager decorations (but allow window movement
  by dragging)
