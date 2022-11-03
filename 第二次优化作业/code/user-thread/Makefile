CFLAGS = -g -I ./include/

main: scheduler timer user-thread context
	gcc ./main.c ./build/timer.o ./build/scheduler.o ./build/user-thread.o ./build/context.o -o ./main $(CFLAGS)

timer: timer.c
	gcc timer.c -c -o ./build/timer.o  $(CFLAGS)

user-thread: user-thread.c
	gcc user-thread.c -c -o ./build/user-thread.o $(CFLAGS)

scheduler: scheduler.c context
	gcc scheduler.c -c -o ./build/scheduler.o $(CFLAGS)

context:
	as context.s -o ./build/context.o

clean:
	rm ./build/*.o
	rm main

gdb:
	gdb main