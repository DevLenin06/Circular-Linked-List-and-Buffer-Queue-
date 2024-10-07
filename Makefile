all: driver.out

driver.out: driver.o buffer.o bufferlist.o
	g++ -Wall -std=c++11 driver.o buffer.o bufferlist.o -o driver.out

driver.o: driver.cpp bufferlist.h
	g++ -Wall -std=c++11 -c driver.cpp

buffer.o: buffer.cpp buffer.h
	g++ -Wall -std=c++11 -c buffer.cpp

bufferlist.o: bufferlist.cpp bufferlist.h buffer.h
	g++ -Wall -std=c++11 -c bufferlist.cpp


clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~*.hgch *#
	rm -f *~ *.h.gch *#

val:
	valgrind ./driver

run: 
	./driver.out