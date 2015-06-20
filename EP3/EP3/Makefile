ep3: main.o monitor.o thread.o
	g++-4.7 -pthread -std=c++11 main.o monitor.o thread.o -o ep3

main.o: main.cpp monitor.h thread.h
	g++-4.7 -Wall -pthread -std=c++11 -c main.cpp 

thread.o: thread.cpp thread.h monitor.h
	g++-4.7 -Wall -pthread -std=c++11 -c thread.cpp

monitor.o: monitor.cpp monitor.h
	g++-4.7 -Wall -std=c++11 -c monitor.cpp


.PHONY: clean
clean:
	rm -f *.o