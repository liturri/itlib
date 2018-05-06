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

SHRLIB=$(BIN)/libitlib.so
SHRLIBVERSION=$(SHRLIB).1

all:  $(SHRLIB) $(BIN)/pru $(BIN)/pru01 # gdbm_api # str2

install:
	mkdir -p $(INSTALL_PATH)/lib $(INSTALL_PATH)/include
	cp -a $(SHRLIBVERSION) $(INSTALL_PATH)/lib
	cp *.hxx $(INSTALL_PATH)/include

$(SHRLIB): $(OBJS)
	$(CXX) $(CXXFLAGS) -lpq -shared -Wl,-soname,$(SHRLIB) -lc -o $(SHRLIBVERSION) $^ -l gdbm
	ln -fs $(SHRLIBVERSION) $(SHRLIB)

$(BIN)/%.o:  $(SOURCE)/%.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

clean:
	rm -f $(OBJS) $(SHRLIBVERSION) $(SHRLIB) .deps

.deps: $(SRCS)
	for S in $(SRCS);do gcc -M -MM -MT $(BIN)/$${S/.cc/.o} $$S;done > .deps

include .deps
