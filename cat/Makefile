CC= gcc -Wextra -Wall -Werror

all: s21_cat

s21_grep:
	$(CC) -o s21_cat s21_cat.c
clean:
	rm -f s21_cat
	rm -f log.txt
	rm -rf *.dSYM
rebuild:
	make clean
	make
test:
	./test.sh
leaks:
	leaks -atExit -- ./s21_cat -benst s21_cat.c
