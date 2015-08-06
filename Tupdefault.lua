tup.foreach_rule('*.cpp', 'g++ -std=c++11 -c %f -o %o -I $(NETHACK_ROOT)/', {'$(NETHACK_ROOT)/temp/%B.o', '$(NETHACK_ROOT)/temp/<objs>'});
