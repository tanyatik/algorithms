default: all

CC=g++
CXXWARN=-Wall
CXXFLAGS=-O3 -g -std=c++11 -fno-omit-frame-pointer
CXXFLAGS+=-I. -isystem ./contrib/gmock
LDFLAGS=
LDLIBS=-lpthread -lglog

# OBJS=$(filter-out cpp/%_main.o,$(patsubst %.cpp,%.o,$(wildcard cpp/*.cpp)))
GTEST_OBJS=contrib/gmock/gmock-gtest-all.o contrib/gmock/gmock_main.o

UT_OBJS=$(patsubst %.cpp,%.o,$(wildcard *_ut.cpp))

TESTS=string_ut heap_ut
UT_BINS=$(patsubst %,bin/%,$(TESTS))

BINS=$(UT_BINS)

COMPILE_RULE=$(CC) $(CXXWARN) $(CXXFLAGS) $< -c -o $@ -MP -MMD -MF deps/$(subst /,-,$@).d
LINK_RULE=$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

bin/%: ut/%.o $(GTEST_OBJS)
	$(LINK_RULE)

%.o: %.cpp
	$(COMPILE_RULE)

all: $(BINS)

clean:
	@rm -f $(BINS)
	@rm -f ./*.o ut/*_ut.o
	@rm -f ./deps/*.d

-include deps/*.d

