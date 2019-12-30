FLAGS = -ansi -pedantic-errors -Wall -Wextra -g

bitarray_test.out: bitarray_test.cpp bitarray.hpp
	g++ $(FLAGS) bitarray_test.cpp -o bitarray_test.out

.PHONY: clean

clean:
	rm -f *.so *.out *.gch *.o cfile cppfile

        
