make all: part1 part2 part3

part1: part1.c
		gcc -o part1 part1.c

part2: part2.c
		gcc -o part2 part2.c 

part3: part3.c 
		gcc -o part3 part3.c -lm

clean: 
		rm -f part1 *.o
		rm -f part2 *.o 
		rm -f part3 *.o

run1: 
		part1 part1testsequence part1-out-test
run2:
		part1 part1sequence part1-output
		md5sum part1-output
run3:
		part2 part2sequence part2-output
		md5sum part2-output
run4: 
		part3 256 4096 1024 part2sequence part3-output
		md5sum part3-output
run5: 
		part3 128 4096 2048 part2sequence part3-output
		md5sum part3-output 