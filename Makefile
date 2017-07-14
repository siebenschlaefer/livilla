CC        = gcc
CFLAGS   += -Wall -Wextra -ggdb
CPPFLAGS +=
LDFLAGS  +=
LDLIBS   += -lcriterion

PROGRAMS = \
	char_literal_test \
	string_literal_test \
	array_literal_test \
	string_array_literal_test \
	int_array_literal_test \
	deallocator_test \

LIBRARIES =

all: $(LIBRARIES) $(PROGRAMS)

check: all
	./char_literal_test
	./string_literal_test
	./array_literal_test
	./int_array_literal_test
	./string_array_literal_test
	./deallocator_test

#example: example.o obj2.o
char_literal_test: char_literal_test.o char_literal.o
string_literal_test: string_literal_test.o string_literal.o
array_literal_test: array_literal_test.o array_literal.o
string_array_literal_test: string_array_literal_test.o string_array_literal.o array_literal.o string_literal.o
int_array_literal_test: int_array_literal_test.o int_array_literal.o array_literal.o
deallocator_test: deallocator_test.o deallocator.o

$(LIBRARIES): %.a:
	ar -rv $@ $^
	ranlib $@

$(PROGRAMS): %: %.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

%.o: %.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

%.o: %.C
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

%: %.rl
	ragel -o $@ $<

.PHONY: all check clean
clean:
	rm -f $(PROGRAMS) core *.o .*.dep

.%.dep: %.c
	@$(CC)  $(CFLAGS)   -M $(CPPFLAGS) $< > $@

.%.dep: %.cc
	@$(CXX) $(CXXFLAGS) -M $(CPPFLAGS) $< > $@

.%.dep: %.cpp
	@$(CXX) $(CXXFLAGS) -M $(CPPFLAGS) $< > $@

.%.dep: %.cxx
	@$(CXX) $(CXXFLAGS) -M $(CPPFLAGS) $< > $@

.%.dep: %.C
	@$(CXX) $(CXXFLAGS) -M $(CPPFLAGS) $< > $@

-include $(patsubst   %.c,.%.dep, $(wildcard   *.c))
-include $(patsubst  %.cc,.%.dep, $(wildcard  *.cc))
-include $(patsubst %.cpp,.%.dep, $(wildcard *.cpp))
-include $(patsubst %.cxx,.%.dep, $(wildcard *.cxx))
-include $(patsubst   %.C,.%.dep, $(wildcard   *.C))

