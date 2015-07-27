default: all

CC=g++
CXXWARN=-Wall
CXXFLAGS=-O0 -g -std=c++11 -fno-omit-frame-pointer
CXXFLAGS+=-I. -isystem ./contrib/gmock
LDFLAGS=-g
LDLIBS=-lpthread -lglog

GTEST_OBJS=contrib/gmock/gmock-gtest-all.o contrib/gmock/gmock_main.o
UT_OBJS=$(patsubst %.cpp,%.o,$(wildcard *_ut.cpp))

LSECTIONS=string math sort
HSECTIONS=heap tree hash

SECTIONS=$(LSECTIONS) $(HSECTIONS)

LOBJS=$(foreach dir,$(LSECTIONS),$(patsubst %.cpp,%.o,$(wildcard $(dir)/*.cpp)))

TESTS=$(patsubst %,%_ut,$(SECTIONS))
UT_BINS=$(patsubst %,bin/%,$(TESTS))

BINS=$(UT_BINS)

COMPILE_RULE=$(CC) $(CXXWARN) $(CXXFLAGS) $< -c -o $@ -MP -MMD -MF deps/$(subst /,-,$@).d
LINK_RULE=$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

bin/%: ut/%.o $(GTEST_OBJS) $(LOBJS)
	$(LINK_RULE)

%.o: %.cpp
	$(COMPILE_RULE)

all: $(BINS)

clean:
	@rm -f $(BINS)
	@rm -f ./*.o ut/*_ut.o
	@rm -f ./deps/*.d

run_tests:
	./bin/hash_ut
	./bin/heap_ut
	./bin/sort_ut
	./bin/string_ut
	./bin/tree_ut
	./bin/math_ut

-include deps/*.d

