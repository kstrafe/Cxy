CPPFLAGS=g++ -std=c++11 -g
INCLUDE=-Ilibraries -I.

all:
	mkdir -p binaries
	$(CPPFLAGS) code/main.cpp -o binaries/tulc.exe $(INCLUDE)

test:
	./tests/test_runner.sh
	$(CPPFLAGS) code/test.cpp -o binaries/tulc_test.exe $(INCLUDE)
	./binaries/tulc_test.exe
