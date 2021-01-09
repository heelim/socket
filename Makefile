all:
	cd ./src; $(MAKE);

clean:
	cd ./src; $(MAKE) clean;
	rm -rf bin lib;