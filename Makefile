LANG=C

BIN=$(shell pwd)/bin/
SOURCE=$(shell pwd)

CC=gcc
CXX=g++
#CC=clang
#CXX=clang++
CXXFLAGS=-Wall -g -fPIC -std=c++14
LDFLAGS:=-L$(BIN) -litlib
# C++ link
LINK.o = $(LINK.cc)

SRCS=pru01.cc pru.cc system.cc dates.cc str2.cc gdbm_api.cc pgdb.cc
OBJS=system.o dates.o str2.o gdbm_api.o pgdb.o
OBJS := $(foreach O,$(OBJS),$(BIN)/$(O)) 

INSTALL_PATH=/usr/local/

SHRLIBNAME=libitlib.so
SHRLIBPATH=$(BIN)/$(SHRLIBNAME)
SHRLIBVERSION=$(BIN)/$(SHRLIBNAME).1

all:  $(SHRLIBPATH) $(BIN)/pru $(BIN)/pru01 # gdbm_api # str2

install:
	mkdir -p $(INSTALL_PATH)/lib $(INSTALL_PATH)/include
	cp -a $(SHRLIBVERSION) $(INSTALL_PATH)/lib/
	cp *.hxx $(INSTALL_PATH)/include

$(SHRLIBPATH): $(OBJS)
	$(CXX) $(CXXFLAGS) -lpq -shared -Wl,-soname,$(SHRLIBNAME) -lc -o $(SHRLIBVERSION) $^ -l gdbm
	ln -fs $(SHRLIBVERSION) $(SHRLIBPATH)

$(BIN)/%.o:  $(SOURCE)/%.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

clean:
	rm -f $(OBJS) $(SHRLIBVERSION) $(SHRLIBPATH) .deps

.deps: $(SRCS)
	for S in $(SRCS);do gcc -M -MM -MT $(BIN)/$${S/.cc/.o} $$S;done > .deps

include .deps
