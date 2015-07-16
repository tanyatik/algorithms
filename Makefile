default: all

CC=g++
CXXWARN=-Wall
CXXFLAGS=-O3 -g -std=c++11 -fno-omit-frame-pointer
CXXFLAGS+=-I. -isystem ./contrib/gmock
LDFLAGS=
LDLIBS=-lpthread -lglog

GTEST_OBJS=contrib/gmock/gmock-gtest-all.o contrib/gmock/gmock_main.o
UT_OBJS=$(patsubst %.cpp,%.o,$(wildcard *_ut.cpp))

LSECTIONS=string
HSECTIONS=heap tree

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

-include deps/*.d

