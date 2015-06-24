all:
	mkdir -p binaries
	g++ code/main.cpp -o binaries/tulc.exe

test:
	./tests/test_runner.sh
	
