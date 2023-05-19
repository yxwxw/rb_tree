build: rbtree.h rbtree_test.cpp
	g++ rbtree_test.cpp -g -o rbtree_test -Wall -Werror && ./rbtree_test

clean:
	rm -r *.o 