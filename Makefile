CC=gcc
LD=g++-8
SWIG=swig
SWIGDIR=swig/
BOOSTDIR=boost_wrap/
OBJDIR=obj/
SRCDIR=src/
INCDIR=include/
TESTDIR=tests/
CCFLAGS = -c -fpic -std=c++17
SWIGFLAGS=-c++ -python -py3 -builtin 
STRICTFLAGS =  -Wall -Wpedantic -Werror
LDFLAGS=-shared -Wl,--no-allow-shlib-undefined
SOURCES:= $(SWIGDIR)my_string_wrap.cxx $(SRCDIR)my_string.cpp
OBJECTS:=$(addprefix $(OBJDIR),$(notdir $(addsuffix .o,$(basename $(SOURCES)))))
ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))/
#modify if needed...
PYTHON_INCLUDE=-I/usr/include/python3.6m
PYTHON_LIB=-I/usr/lib/x86_64-linux-gnu
swig:_my_string_swig.so
	mv $(SWIGDIR)_my_string_swig.so $(TESTDIR)py/ 
	mv $(SWIGDIR)my_string_swig.py $(TESTDIR)py/ 
boost:$(OBJDIR)boost_wrap.o $(OBJDIR)my_string.o
	$(LD) $(LDFLAGS) $^ -L$(ROOT_DIR)$(BOOSTDIR) -lpython3.6m -lboost_python36   -std=c++17 -o $(BOOSTDIR)my_string_boost.so 
	mv  $(BOOSTDIR)my_string_boost.so  $(TESTDIR)py/ 
_my_string_swig.so: make_wrap $(OBJECTS)
	$(LD) $(LDFLAGS)  $(OBJECTS) $(PYTHON_INCLUDE) -o $(SWIGDIR)$@ 
make_wrap:$(SWIGDIR)my_string.i
	$(SWIG) $(SWIGFLAGS) $<
$(OBJDIR)my_string.o:$(SRCDIR)my_string.cpp
	$(CC) $(PYTHON_INCLUDE) $(STRICTFLAGS) $(CCFLAGS) $< -o $@
$(OBJDIR)my_string_wrap.o:$(SWIGDIR)my_string_wrap.cxx
	$(CC) $(PYTHON_INCLUDE) $(CCFLAGS) $< -o $@ 
$(OBJDIR)boost_wrap.o:$(BOOSTDIR)boost_wrap.cpp
	$(CC) $(CCFLAGS) $< -o $@ $(PYTHON_INCLUDE) 
clean:
	@rm -f $(OBJECTS) $(SWIGDIR)_my_string_swig.so
	@rm -f $(TESTDIR)run_tests.out
	@rm -f $(OBJDIR)boost_wrap.o
	@rm -f $(TESTDIR)py/_my_string_swig.so 
	@rm -f $(TESTDIR)py/my_string_swig.py 
	@rm -f $(TESTDIR)py/my_string_boost.so
	@rm -f $(SWIGDIR)my_string_wrap.cxx
test: $(TESTDIR)run_tests.cpp $(OBJDIR)my_string.o
	$(CC) $(CCFLAGS) $(STRICTFLAGS) $< -o $(basename $<).o
	@$(LD) $(addsuffix .o,$(basename $^)) -o $(basename $<).out
	@rm $(basename $<).o
	$(basename $<).out
	@rm $(basename $<).out
test_py:boost swig
	python3.6 $(TESTDIR)py/swig_tests.py
	python3.6 $(TESTDIR)py/boost_tests.py