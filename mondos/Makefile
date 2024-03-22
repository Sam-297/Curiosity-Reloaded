CC= clang -Wall -g

all: mestests

%.o:%.c 
	$(CC) -c $<

mestests.o: mestests.c meslistes.h

meslistes.o: meslistes.c meslistes.h

mestests: mestests.o meslistes.o
	$(CC) $^ -o $@

clean:
	rm -f mestests *.o