all: part1 part2

part1: part1.c 
	gcc part1.c -lm -o part1 -lpthread
	
part2: part2.java
	javac part2.java

run1: 
	./part1 1000

run2: 
	./part1 1000
	./part1 5000
	./part1 10000
	./part1 20000

run3: 
	java part2 1000

run4: 
	java part2 1000
	java part2 5000
	java part2 10000
	java part2 20000
	
clean :
	rm -f *.class
	rm -f part1
%.class : part2.java
	$(JAVAC) $<