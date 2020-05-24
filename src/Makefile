PROGRAM = wasmp
ARGUMENTS = 

CC = g++
CPPFLAGS = -pedantic -Wall -Wextra -std=c++11 -O3

LD = g++
LDFLAGS = -lm

CPPFILES = $(wildcard *.cpp)
OFILES = $(CPPFILES:.cpp=.o)

all: depend $(PROGRAM)


$(PROGRAM): $(OFILES)
	$(LD) -o $@ $(OFILES) $(LDFLAGS)

.PHONY: all clean run depend

clean:
	rm -f $(OFILES) $(PROGRAM)

run: $(PROGRAM)
	./$(PROGRAM) $(ARGUMENTS)

DEPENDFILE = .depend

depend:
	$(CC) $(CPPFLAGS) -MM $(CPPFILES) > $(DEPENDFILE)


-include $(DEPENDFILE)