FLTK_CXXFLAGS = -I/usr/lib/fltk -std=gnu++17
#FLTK_LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lfltk_images -lfltk_forms -lfltk_gl -lfltk -lSM -lICE -lX11 -lXext -lm
FLTK_LDFLAGS = -lfltk_images -lfltk_forms -lfltk_gl -lfltk -lSM -lICE -lX11 -lXext -lm

CXX = g++
CXXFLAGS = -g -Wall -Iinclude $(FLTK_CXXFLAGS)

SRCS = main.cpp cpu.cpp
OBJS = $(SRCS:%.cpp=build/%.o)

EXE = build/coremon3

build/%.o : src/%.cpp
	$(CXX) $(CXXFLAGS) -c src/$*.cpp -o build/$*.o

$(EXE) : $(OBJS)
	$(CXX) -o $@ $+ $(FLTK_LDFLAGS)

clean :
	rm -f build/*.o $(EXE)

depend :
	$(CXX) $(CXXFLAGS) -M $(SRCS:%=src/%) > depend.mak

depend.mak :
	touch $@

include depend.mak
