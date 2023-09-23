
ls: myls.c
	gcc -Wall -pedantic -o ls myls.c

.PHONY: clean
clean:
	rm -f ls
