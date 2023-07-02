CC= gcc -Wextra -Wall -Werror

all: s21_grep

s21_grep:
	$(CC) -o s21_grep s21_grep.c
clean:
	rm -f s21_grep
	rm -f log.txt 
	rm -rf *.dSYM
rebuild:
	make clean
	make

test:
	./t_grep_mac.sh

leaks:
	leaks -atExit -- ./s21_grep -e hhh -f pattern.txt Makefile s21_grep.c

valgrind:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-reachable=yes --num-callers=20 --track-fds=yes ./s21_grep -e include -f pattern.txt s21_cat.c Makefile


