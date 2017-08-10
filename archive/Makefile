# You might need to read http://libcxx.llvm.org/ and/or
# related Q&A on StackOverflow to get C++11 working.

DEBUG	 = 0

LLVM_PATH = /usr/local/Cellar/llvm/4.0.1/bin/

CC       = ${LLVM_PATH}clang
CCFLAGS  = -g -O2 -std=c11 -DDEBUG=$(DEBUG)
CLIBS    =

CXX      = ${LLVM_PATH}clang++
CXXFLAGS = -g -Os -std=c++11 -stdlib=libc++ -DDEBUG=$(DEBUG)
CXXFLAGS += -Rpass-analysis=loop-vectorize -Rpass=loop-vectorize
CXXFLAGS += -march=x86-64 #-mcpu=haswell #-mattr=+avx2
CXXLIBS  = -nodefaultlibs -lc++ -lc++abi -lm -lc

LD       = $(CC)
LDFLAGS  = $(CFLAGS)

TESTS = test-kahan-allreduce test-kahan-summation test-kahan-wrapper basic

all: $(TESTS)

%.c.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

%.cc.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%: %.c.o
	$(CC) $(CFLAGS) $< $(CLIBS) -o $@

%: %.cc.o
	$(CXX) $(CXXFLAGS) $< $(CXXLIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TESTS)
	-rm -rf *.dSYM

