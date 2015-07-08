CPPFLAGS=g++ -Wall -std=c++11 -g
SOURCES=`find -name *.cpp | grep -Pv test.*?\.cpp`
TEST_SOURCES=`find -name *.cpp | grep -Pv main.*?\.cpp`
INCLUDE=-I.

all:
	mkdir -p binaries
	$(CPPFLAGS) $(SOURCES) -o binaries/tulc.exe $(INCLUDE)

gen:
	python3 ./codegenerators/CrossTerminal.py

test:
	./tests/test_runner.sh
	$(CPPFLAGS) $(TEST_SOURCES) -o binaries/tulc_test.exe $(INCLUDE)
	./binaries/tulc_test.exe
