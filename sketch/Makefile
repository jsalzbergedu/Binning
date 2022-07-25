.PHONY: all
all: sketch sketch_2 sketch_3

sketch: sketch.c
	cc -g -O0 sketch.c -std=c99 -o sketch

sketch_2: sketch_2.c
	cc -g -O0 sketch_2.c -std=c99 -o sketch_2

sketch_3: sketch_3.c
	cc -g -O0 sketch_3.c -std=c99 -o sketch_3

.PHONY: clean
clean:
	rm -rf sketch sketch_2 sketch_3
