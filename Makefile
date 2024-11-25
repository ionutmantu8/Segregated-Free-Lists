build:
	gcc sfl.c functions.c -g -Wall -Wextra -std=c99 -o sfl
clean:
	rm sfl
run:
	./sfl
pack: 
	zip -r cs.zip README.md cs/cs.sh cs/checkpath.pl cs/spelling.txt cs/const_structs.
.PHONY: build pack clean