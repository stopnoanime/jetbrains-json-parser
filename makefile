.PHONY: all clean test

all: 
	mkdir -p build
	cd build ; cmake ../src/ ; cmake --build .

test: all
	./build/tests/tests

clean:
	rm -rf build