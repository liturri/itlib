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
#LINK.o = $(LINK.cc)

SRC := pru01.cc pru.cc
OBJ := $(SRC:cc=o)
LIB_SRC := system.cc dates.cc str2.cc gdbm_api.cc pgdb.cc
LIB_OBJ := $(LIB_SRC:cc=o)
LIB_OBJ := $(foreach O,$(LIB_OBJ),$(BIN)/$(O)) 

ALL_SRC := $(SRC) $(LIB_SRC)
ALL_OBJ := $(OBJ) $(LIB_OBJ)

INSTALL_PATH=/usr/local/

SHRLIBNAME=libitlib.so
SHRLIBPATH=$(BIN)/$(SHRLIBNAME)
SHRLIBVERSION=$(BIN)/$(SHRLIBNAME).1

all: $(SHRLIBPATH) $(BIN)/pru $(BIN)/pru01 # gdbm_api # str2

install:
	mkdir -p $(INSTALL_PATH)/lib $(INSTALL_PATH)/include
	cp -a $(SHRLIBVERSION) $(INSTALL_PATH)/lib/
	cp *.hxx $(INSTALL_PATH)/include

$(SHRLIBPATH): $(LIB_OBJ)
	$(CXX) $(CXXFLAGS) -lpq -shared -Wl,-soname,$(SHRLIBNAME) -lc -o $(SHRLIBVERSION) $^ -l gdbm
	ln -fs $(SHRLIBVERSION) $(SHRLIBPATH)

$(BIN)/%.o:  $(SOURCE)/%.cc
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

$(BIN)/%:  $(BIN)/%.o
	$(LINK.cc) $(OUTPUT_OPTION) $<

clean:
	rm -f $(ALL_OBJ) $(SHRLIBVERSION) $(SHRLIBPATH) .deps

.deps: $(ALL_SRC)
	for S in $(ALL_SRC);do gcc -M -MM -MT $(BIN)/$${S/.cc/.o} $$S;done > .deps

include .deps
