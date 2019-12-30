FLAGS = -ansi -pedantic-errors -Wall -Wextra -g

shared_ptr_test.out: shared_ptr_test.cpp shared_ptr.hpp
	g++ $(FLAGS) shared_ptr_test.cpp -o shared_ptr_test.out

.PHONY: clean

clean:
	rm -f *.so *.out *.gch *.o cfile cppfile

        
