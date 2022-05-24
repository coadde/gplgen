all: gplgen.c

gplgen.c:
	cc -o src/gplgen_c -std=c17 src/gplgen.c
