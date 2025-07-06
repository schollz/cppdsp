do: build 
	cd build && make -j 32
	cd build && ./plot_envelope
	python plot.py 

build:
	mkdir -p build 
	cd build && cmake ..

clean:
	rm -rf build
	rm -f plot_envelope
	rm -f plot.png