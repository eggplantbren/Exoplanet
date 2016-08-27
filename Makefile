CFLAGS = -std=c++11 -O3 -DNDEBUG -Wall -Wextra -pedantic
LIBS = -ldnest4 -lgsl -lgslcblas

default:
	g++ $(CFLAGS) -I$(DNEST4_PATH) -c *.cpp
	g++ -pthread -L$(DNEST4_PATH)/DNest4/code -o main *.o $(LIBS)
	rm -f *.o

