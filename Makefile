appname := main

INC_add := -I/usr/include -I./glvu/ 
LINK_add := -L/usr/lib/x86_64-linux-gnu/ -lGL -lglut -lGLU

CXX := clang++
CXXFLAGS := -std=c++14 -g -Wall -O3 $(INC_add) $(LINK_add)


srcfiles := main.cpp ./glvu/glvu.cpp
objects  := $(%.C, %.o )
headers  := $(shell find . -name "*.h")

all: $(appname)

$(appname): $(objects) $(srcfiles) $(headers)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(srcfiles) $(objects) $(LDLIBS) -o $(appname)

depend: .depend

.depend: $(srcfiles) 
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	-rm -f $(objects)
	-rm -f main

dist-clean: clean
	rm -f *~ .depend

include .depend

