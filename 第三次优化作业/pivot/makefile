CFLAGS = -fopenmp -mavx2 -lm -g -Og

pivot: ./build/pivot.o ./build/heap.o
	gcc ./build/pivot.o ./build/heap.o -o pivot $(CFLAGS)

./build/pivot.o: ./src/pivot.c
	gcc ./src/pivot.c -c -o ./build/pivot.o $(CFLAGS)

./build/heap.o: ./src/heap.c
	gcc ./src/heap.c -c -o ./build/heap.o $(CFLAGS)

run: pivot
	./pivot

clean:
	rm ./build/*