CFLAGS = -O2 -Wall -Wextra -ansi -pedantic
LIBS =  -ldnest3 -lgsl -lgslcblas -lboost_system -lboost_thread
OBJECTS = Lookup.o

default:
	g++ $(CFLAGS) -c *.cpp
	g++ -o Orbit Orbit.o $(OBJECTS) $(LIBS)
	rm -f *.o

