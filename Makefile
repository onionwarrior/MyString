CC=gcc
LD=g++-8
SWIG=swig
SWIGDIR=swig/
OBJDIR=obj/
SRCDIR=src/
INCDIR=include/
TESTDIR=tests/
CCFLAGS = -c -fpic -std=c++17
SWIGFLAGS=-c++ -python -py3 -builtin 
STRICTFLAGS =  -Wall -Wpedantic -Werror
LDFLAGS=-shared
SOURCES:= $(SWIGDIR)my_string_wrap.cxx $(SRCDIR)my_string.cpp
OBJECTS:=$(addprefix $(OBJDIR),$(notdir $(addsuffix .o,$(basename $(SOURCES)))))
#modify if neededl
PYTHON_INCLUDE= -I/usr/include/python3.6
PYTHON_LIB=-I/usr/lib/x86_64-linux-gnu
_my_string_swig.so: make_wrap $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(SWIGDIR)$@ 
make_wrap:$(SWIGDIR)my_string.i
	$(SWIG) $(SWIGFLAGS) $<
$(OBJDIR)my_string.o:$(SRCDIR)my_string.cpp
	$(CC) $(STRICTFLAGS) $(CCFLAGS) $< -o $@
$(OBJDIR)my_string_wrap.o:$(SWIGDIR)my_string_wrap.cxx
	$(CC) $(CCFLAGS) $< -o $@ $(PYTHON_INCLUDE)
clean:
	rm $(OBJECTS) $(SWIGDIR)_my_string_swig.so
	rm $(TESTDIR)run_tests.out
test: $(TESTDIR)run_tests.cpp $(OBJDIR)my_string.o
	$(CC) $(CCFLAGS) $(STRICTFLAGS) $< -o $(basename $<).o
	$(LD) $(addsuffix .o,$(basename $^)) -o $(basename $<).out
	rm $(basename $<).o
	$(basename $<).out
	rm $(basename $<).out