all: main
	g++ test.cpp -lcom -L../lib -I../include -o test

main:
	$(MAKE) -C ../;

clean: 
	rm -rf test
