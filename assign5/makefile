all: prog

prog: prog.c 
		gcc -o prog prog.c -lpthread 

clean: 
		rm -f prog *.o

run1: 
		prog -alg FIFO -input input1.txt 
run2: 
		prog -alg SJF -input input1.txt 
run3: 
		prog -alg PR -input input1.txt 
run4: 
		prog -alg RR -quantum 10 -input input1.txt
run5: 
		prog -alg RR -quantum 20 -input input1.txt 
run6: 
		prog -alg RR -quantum 30 -input input1.txt
		

