CC=g++
#CC=g++-5
#CFLAGS=-Wall -g -std=c++0x
CFLAGS=-Wall -g -std=c++17

.SUFFIXES: .o .cpp .h

SRC_DIRS = ./ ./benchmarks/ ./concurrency_control/ ./storage/ ./system/
#INCLUDE = -I. -I./benchmarks -I./concurrency_control -I./storage -I./system
INCLUDE = -I. -I./benchmarks -I./concurrency_control -I./storage -I./system -I../cicada-engine/src -I../cicada-engine/third_party/abseil-cpp -I..

#CFLAGS += $(INCLUDE) -D NOGRAPHITE=1 -Werror -O3
CFLAGS += $(INCLUDE) -D NOGRAPHITE=1 -Wno-unused-function -O3 #-fno-omit-frame-pointer
#CFLAGS += $(INCLUDE) -D NOGRAPHITE=1 -Wno-unused-function -Og -fno-omit-frame-pointer
#CFLAGS += $(INCLUDE) -D NOGRAPHITE=1 -Wno-unused-function -O0 -fno-omit-frame-pointer

#LDFLAGS = -Wall -L. -L./libs -pthread -g -lrt -std=c++0x -O3 -ljemalloc
#LDFLAGS = -Wall -L. -L./libs -L../cicada-engine/build -pthread -g -lrt -std=c++14 -lcommon -lnuma -ljemalloc -O3
LDFLAGS = -L. -L../cicada-engine/build -pthread -lrt -lcommon -lnuma -ljemalloc
LDFLAGS += $(CFLAGS)

CPPS = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)*.cpp))
OBJS = $(CPPS:.cpp=.o)
DEPS = $(CPPS:.cpp=.d)

all:rundb

rundb : $(OBJS) ../cicada-engine/build/libcommon.a \
        ../cicada-engine/build/libabsl_hash.a \
        ../cicada-engine/build/libabsl_city.a \
        ../cicada-engine/build/libabsl_wyhash.a \
        ../cicada-engine/build/libabsl_raw_hash_set.a \
	../silo/out-perf.masstree/allocator.o \
	../silo/out-perf.masstree/compiler.o \
	../silo/out-perf.masstree/core.o \
	../silo/out-perf.masstree/counter.o \
	../silo/out-perf.masstree/json.o \
	../silo/out-perf.masstree/straccum.o \
	../silo/out-perf.masstree/string.o \
	../silo/out-perf.masstree/ticker.o \
	../silo/out-perf.masstree/rcu.o
	$(CC) -o $@ $^ $(LDFLAGS)

-include $(OBJS:%.o=%.d)

%.d: %.cpp
	$(CC) -MM -MT $*.o -MF $@ $(CFLAGS) $<

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f rundb $(OBJS) $(DEPS)
