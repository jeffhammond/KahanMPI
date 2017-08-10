# You might need to read http://libcxx.llvm.org/ and/or
# related Q&A on StackOverflow to get C++11 working.

DEBUG	 = 0

CC       = clang
CCFLAGS  = -g -O2 -std=c11 -DDEBUG=$(DEBUG)
CLIBS    =

CXX      = clang++
CXXFLAGS = -g -O2 -std=c++11 -stdlib=libc++ -DDEBUG=$(DEBUG) -Rpass-analysis
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

