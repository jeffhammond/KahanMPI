# You might need to read http://libcxx.llvm.org/ and/or
# related Q&A on StackOverflow to get C++11 working.

CC       = clang
CCFLAGS  = -g -O2 -std=c11 -DDEBUG=$(DEBUG)

CXX      = clang++
CXXFLAGS = -g -O2 -std=c++11 -stdlib=libc++ -DDEBUG=$(DEBUG)
CXXLIBS  = -nodefaultlibs -lc++ -lc++abi -lm -lc

LD       = $(CC)
LDFLAGS  = $(CFLAGS)

TESTS = test-kahan-allreduce test-kahan-summation test-kahan-wrapper

all: $(TESTS)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%: %.o
	$(CXX) $(CXXFLAGS) $< $(CXXLIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TESTS)
	-rm -rf *.dSYM

