CC=g++
CFLAGS=-g -O2 -Wall
LINKLUA = -llua

all:
	make libLoadLuaConfigure.a 
	make main
	
libLoadLuaConfigure.a:\
	LoadLuaConfigure.o 
	ar -crv -o $@ $^ 
	
main:\
	main.o
	$(CC) -o $@ $^ -L . -lLoadLuaConfigure $(LINKLUA)
	
clean:
	rm -f libloadLuaConfigure.a
	rm -f main
	rm -f *.o
.c.cpp.o:
	$(CC) $(CFLAGS) -c -o $*.o $<
	
