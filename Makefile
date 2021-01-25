
all: lib
	g++ main.cpp -L. -lpthread -lqueue;

lib: queue

queue:
	g++ Queue.cpp -c -o libqueue.a

clean:
	rm -rf a.out libqueue.a;
