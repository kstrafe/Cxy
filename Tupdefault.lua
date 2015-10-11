tup.foreach_rule('*.cpp', 'g++ -std=c++11 -c %f -o %o -I $(ROOT)/', {'$(ROOT)/temp/%B.o', '$(ROOT)/temp/<objs>'});
