build:
	gcc -g -Wall -Wextra my_octave.c -o my_octave

run:
	./my_octave
	
clean:
	rm -f my_octave

.PHONY: clean