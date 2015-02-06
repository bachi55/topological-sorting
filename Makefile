# Different between '=' and ':='
# 'var  =' ... evaluated recursively every time 'var' is called
# 'var :=' ... evaluated just ones when 'var' is defined

#TODO: Make output depend on the templates.

# -pg (for compiling and linking) needed for profiling with gprof
# disable inlining for profiling

CXX 		:= g++
LD 		:= g++
MKDIR 		:= mkdir
RM		:= rm -rf
CXXFLAGS 	 = -std=c++11 -Wall
OPTIFLAGS 	:= -O3 
DEBUGFLAGS 	:= -g
ifeq ($(BUILD), debug)
	CXXFLAGS += $(DEBUGFLAGS)
else ifeq ($(BUILD), profiling)
	CXXFLAGS += -pg -fno-inline
	LDFLAGS  += -pg
else
	CXXFLAGS += $(OPTIFLAGS)
endif

CXXFLAGS += -Iinclude
# CXXFLAGS += -I../tools/

# Flags related to the google-test
CXXGTESTFLAGS 	:= -isystem $(GTEST_DIR)/include -pthread 
LDGTESTFLAGS 	:= -L$(GTEST_DIR)/lib -lgtest -pthread

SOURCES 	:= $(wildcard src/*.cpp)
OBJECTS 	:= $(patsubst %.cpp, %.o, $(SOURCES))
 
SOURCES_BUILD 	:= $(wildcard src/build/*.cpp)
OBJECTS_BUILD 	:= $(patsubst %.cpp, %.o, $(SOURCES_BUILD))

SOURCES_TEST 	:= $(wildcard src/unittest/*.cpp)
OBJECTS_TEST 	:= $(patsubst %.cpp, %.o, $(SOURCES_TEST))

OUT 		:= bin
BINARY_BUILD 	:= $(OUT)/topological-sort
BINARY_TEST 	:= $(OUT)/topological-sort_unittest

MEASUREMENTS_OUT:= measurements

.PHONY: all
all : build

.PHONY: build
build : CXXFLAGS += -DNDEBUG
build : $(BINARY_BUILD)


.PHONY: tests
tests : CXXFLAGS += $(CXXGTESTFLAGS)
tests : LDFLAGS  += $(LDGTESTFLAGS)
tests : $(BINARY_TEST)
# 	@echo $(BINARY_TEST)
# 	$(foreach binary, $(BINARY_TEST), echo $(binary);)
# 	$(foreach binary, $(BINARY_TEST), exec $(binary) --gtest_filter=correctness*:feature*;)
# TODO: How to handle to execute more than one binary? 
	@exec $(BINARY_TEST) --gtest_filter=correctness*:feature*

.PHONY: measure
measure : CXXFLAGS += $(CXXGTESTFLAGS) -DNDEBUG
measure : LDFLAGS  += $(LDGTESTFLAGS)
measure : $(BINARY_TEST) | $(MEASUREMENTS_OUT)/
	@exec $(BINARY_TEST) --gtest_filter=measurements*

# | reffers to a 'order-only' prerequisite. It requiers make 3.80!
# Source: http://www.cmcrossroads.com/article/making-directories-gnu-make?page=0%2C1
$(BINARY_BUILD) : $(OBJECTS) $(OBJECTS_BUILD) | $(OUT)/
	$(LD) $^ $(LDFLAGS) -o $@

$(BINARY_TEST) : $(OBJECTS) $(OBJECTS_TEST) | $(OUT)/
	$(LD) $^ $(LDFLAGS) -o $@

$(OUT) :
	$(MKDIR) $@

$(MEASUREMENTS_OUT) :
	$(MKDIR) $@

# clean targets to clean-up the directories
.PHONY: clean
clean :
	$(RM) $(OUT) $(OBJECTS) $(OBJECTS_BUILD) $(OBJECTS_TEST)
	
.PHONY: clean-measurements
clean-measurements : 
	$(RM) $(MEASUREMENTS_OUT)
	
.PHONY: todolist
todolist:
	@exec grep --after-context=1 --color=auto "TODO:" -r .