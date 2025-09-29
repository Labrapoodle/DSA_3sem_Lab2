CFLAGS = -Wall -Werror -Wextra 
CPPFLAGS =  -MMD -I includes/ 

SRC = src/

.PHONY: all
all: Lab2

-include main.d rbtree.d rtree.d

Lab2: build/main.o build/librbtree.a build/librtree.a 
	gcc -Iincludes/   $(CFLAGS)  $^ -o $@
	rm build/main.o;
	rm build/main.d;
	rm build/rbtree.o;
	rm build/rbtree.d ;
	rm build/rtree.o;
	rm build/rtree.d ;

build/main.o: src/main.c
	gcc -c $(CFLAGS) $(CPPFLAGS) $< -o $@

build/librbtree.a: build/rbtree.o build/rtree.o
	ar rcs $@ $^

build/rbtree.o: src/rbtree.c
	gcc -c  $(CPPFLAGS) $< -o $@

build/librtree.a: build/rtree.o build/rbtree.o
	ar rcs $@ $^

build/rtree.o:  src/rtree.c
	gcc -c  $(CPPFLAGS) $< -o $@



.PHONY: clean
clean:
	rm build/main.o;
	rm build/main.d;
	rm build/rbtree.o;
	rm build/rbtree.d;
	rm build/rtree.o;
	rm build/rtree.d;		
	rm build/librbtree.a;
	rm build/librtree.a;

