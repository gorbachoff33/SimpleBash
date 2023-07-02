#include <getopt.h>
#include <stdio.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} flagi;

static struct option flags[] = {
    {"number-nonblank", 0, 0, 'b'},
    {"number", 0, 0, 'n'},
    {"squeeze-blank", 0, 0, 's'},
    {0, 0, 0, 0},
};

int flags_parser(int argc, char *argv[], flagi *flag);
void output(int argc, char *argv[], flagi *flag);

int main(int argc, char *argv[]) {
  int flag1 = 0;
  flagi flag = {0, 0, 0, 0, 0, 0};
  flag1 = flags_parser(argc, argv, &flag);
  if (flag1 != -10) {
    if (flag.b) {
      flag.n = 0;
    }
    output(argc, argv, &flag);
  }
}

int flags_parser(int argc, char *argv[], flagi *flag) {
  int x = 0;
  int y = 0;
  int ukazatel = 0;
  while ((x = getopt_long(argc, argv, "beEnstTv", flags, &ukazatel)) != -1) {
    switch (x) {
      case 'b':
        flag->b = 1;
        break;
      case 'e':
        flag->e = 1;
        flag->v = 1;
        break;
      case 'E':
        flag->e = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 't':
        flag->t = 1;
        flag->v = 1;
        break;
      case 'T':
        flag->t = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case '?':
        y = -10;
        break;
    }
  }
  return y;
}

void output(int argc, char *argv[], flagi *flag) {
  FILE *file;
  int number_str = 1;
  int count_sumbol = 0;
  int flag_N = 0;
  int count = 0;
  char c;
  int fil = 0;
  char last = '\n';
  while (optind < argc) {
    number_str = 1;
    fil = 0;
    file = fopen(argv[optind], "r");
    if (file != NULL) {
      flag_N = 0;
      while (1 && flag_N != -100) {
        c = fgetc(file);
        if (c == EOF) {
          flag_N = -100;
        } else {
          fil = 1;
          if (c != '\n') {
            count_sumbol++;
            count = 0;
          } else {
            count_sumbol = 0;
          }
          if (flag->b && count_sumbol == 1) {
            printf("%6d\t", number_str);
            number_str++;
          }
          if (flag->n && last == '\n') {
            if (flag->s == 0) {
              printf("%6d\t", number_str);
              number_str++;
            } else {
              if (count == 0) {
                printf("%6d\t", number_str);
                number_str++;
              }
            }
          }
          if (flag->s && last == '\n' && c == '\n') {
            if (count == 0) {
              count++;
            } else {
              continue;
            }
          }
          last = c;
          if (flag->e && c == '\n') {
            printf("$");
          }
          if (flag->t && c == '\t') {
            printf("^I");
            continue;
          }
          if (flag->v) {
            if ((c >= 0 && c < 9) || (c > 10 && c < 32)) {
              c = c + 64;
            }
          }
          printf("%c", c);
        }
      }
    } else {
      printf("s21_cat: %s no such file or directory\n", argv[optind]);
      flag_N = -100;
    }
    optind++;
    if (fil == 1) {
      fclose(file);
    }
  }
}
