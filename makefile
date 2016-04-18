CXXFLAG = -g -isystem -Werror -Wextra -std=c++11
CXX = g++

OBJS := *.o

TESTHDRS = -I $(GTEST)/include/
TESTFLAG = $(GTEST)/lib/libgtest_main.a $(GTEST)/lib/libgtest.a -lpthread

record.o: record.cc
	$(CXX) $(CXXFLAG) $(TESTHDRS) -c record.cc

table.o: table.cc
	$(CXX) $(CXXFLAG) $(TESTHDRS) -c table.cc

transcation.o: transcation.cc
	$(CXX) $(CXXFLAG) $(TESTHDRS) -c transcation.cc

engine.o: engine.cc
	$(CXX) $(CXXFLAG) $(TESTHDRS) -c engine.cc

main.o: main.cc engine.h
	$(CXX) $(CXXFLAG) $(TESTHDRS) -c main.cc

hash_table_test.o: hash_table_test.cc hash_table.h
	$(CXX) $(CXXFLAG) $(TESTHDRS) -c hash_table_test.cc

hash_table_test: hash_table_test.o
	$(CXX) $(CXXFLAG) $(TESTFLAG) hash_table_test.o -o hash_table_test

all: main.o engine.o record.o transcation.o table.o
	$(CXX) $(CXXFLAG) $(TESTFLAG) $^ -o perftest

clean:
	rm *.o


