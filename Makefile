
do: build 
	cd build && make -j 32

build:
	mkdir -p build 
	cd build && cmake ..

clean:
	rm -rf build
	rm -rf obj 
	rm -rf *.wav 
	rm -rf drumsynth