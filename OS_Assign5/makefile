CXX = g++
CXXFLAGS = -Wall -Werror -std=c++11
run: guest.o cleaning.o
	$(CXX) main.cpp cleaning.o guest.o -lpthread -o run
guest.o:main.hpp guest.cpp
	$(CXX) $(CXXFLAGS) -c guest.cpp
cleaning.o:main.hpp cleaning.cpp
	$(CXX) $(CXXFLAGS) -c cleaning.cpp
clean:
	rm -f *.o run