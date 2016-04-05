CXXFLAG = -g -Werror -std=c++11
CXX = clang
LIBS =

SRCS := engine_impl.cc table.cc tx_manager.cc
MAIN := perf_main.cc
OBJS := $(SRCS:.cc=.o)
DEPS := $(OBJS:.o=.d)

TESTFLAG = -l$(GTEST)/lib/gtest_main -I $(GTEST)/include


hash_test: hash_table_test.cc hash_table.h
	$(CXX) $(CXXFLAG) $(TESTFLAG) hash_table_test.cc -o hash_table_test

clean:
	rm *.o


