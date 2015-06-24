all:
	mkdir -p binaries
	g++ -std=c++11 code/main.cpp -o binaries/tulc.exe -Ilibraries -I.

test:
	./tests/test_runner.sh
