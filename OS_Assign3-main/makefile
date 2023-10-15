CXX = g++
CXXFLAGS = -Wall -Werror -std=c++11
run: producer.o consumer.o
	$(CXX) main.cpp consumer.o producer.o -o run
producer.o:main.hpp producer.cpp
	$(CXX) $(CXXFLAGS) -c producer.cpp
consumer.o:main.hpp consumer.cpp
	$(CXX) $(CXXFLAGS) -c consumer.cpp
clean:
	rm -f *.o run consumer_?.txt