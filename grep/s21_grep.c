// created by pizpotli
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int count_l;
  int count_c;
  int count_f;
  int count_n;
  int count_optind;
} flags;

static struct option flag[] = {
    {0, 0, 0, 0},
};
int get_opt(int argc, char *argv[], flags *flagi);
void parser(int argc, char *argv[], flags *flagi);
void one_argument(int argc, char *argv[], flags *flagi, char *argument);
int file_argument(char *tmp_line2, int *suc, char *files, flags *flagi);
void output(char *argv[], int success, char *tmp_line, flags *flagi);

int main(int argc, char *argv[]) {
  char *argument = NULL;
  flags flagi = {0};
  int flag_exit_parser = 0;
  flag_exit_parser = get_opt(argc, argv, &flagi);
  if (flag_exit_parser != -10) {
    if (flagi.e == 0 && flagi.f == 0) {
      argument = argv[optind];
      optind++;
      flagi.count_optind = optind;
      flagi.count_f = argc - optind;
      one_argument(argc, argv, &flagi, argument);
    } else {
      flagi.count_f = argc - optind;
      flagi.count_optind = optind;
      parser(argc, argv, &flagi);
    }
  }
  return 0;
}

int get_opt(int argc, char *argv[], flags *flagi) {
  int x = 0, flag_mistake = 0, ukazatel = 0;
  while ((x = getopt_long(argc, argv, "e:ivclnhsf:o", flag, &ukazatel)) != -1) {
    switch (x) {
      case 'e':
        flagi->e = 1;
        break;
      case 'i':
        flagi->i = 1;
        break;
      case 'v':
        flagi->v = 1;
        break;
      case 'c':
        flagi->c = 1;
        break;
      case 'l':
        flagi->l = 1;
        break;
      case 'n':
        flagi->n = 1;
        break;
      case 'h':
        flagi->h = 1;
        break;
      case 's':
        flagi->s = 1;
        break;
      case 'f':
        flagi->f = 1;
        break;
      case 'o':
        flagi->o = 1;
        break;
      case '?':
        flag_mistake = -10;
        break;
    }
  }
  return flag_mistake;
}

void parser(int argc, char *argv[], flags *flagi) {
  int x = 0, e = 0, f = 0, suc = 1, line = 0, cflags = 0, success = 0,
      exit_1 = 0;
  FILE *file = NULL;
  char *tmp_line = NULL, *argument = NULL, *files = NULL;
  size_t len = 0;
  regex_t regex_comp = {0};
  if (flagi->f == 1) {
    flagi->e = 1;
  }
  if (flagi->i == 1) {
    cflags = REG_ICASE;
  }
  while (flagi->count_optind < argc) {
    line = 0;
    flagi->count_c = 0;
    flagi->count_n = 1;
    if ((file = fopen(argv[flagi->count_optind], "r")) == NULL) {
      if (flagi->s == 0) {
        printf("s21_grep:%s:-no such file or directory\n",
               argv[flagi->count_optind]);
      }
    } else {
      while (line != EOF) {
        line = getline(&tmp_line, &len, file);
        if (tmp_line && line != EOF) {
          optind = 1;
          suc = 1;
          while ((x = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
            switch (x) {
              case 'e':
                e = 1;
                break;
              case 'f':
                f = 1;
                break;
            }
            if (optarg != NULL) {
              if (e == 1) {
                argument = optarg;
                e = 0;
                regcomp(&regex_comp, argument, cflags);
                success = regexec(&regex_comp, tmp_line, 0, NULL, 0);
                if (success == 0) {
                  suc = success;
                }
                f = 0;
              }
              if (f == 1) {
                files = optarg;
                f = 0;
                exit_1 = file_argument(tmp_line, &suc, files, flagi);
                if (exit_1 == -10) {
                  line = EOF;
                }
              }
              regfree(&regex_comp);
            }
          }
          if (exit_1 != -10) {
            output(argv, suc, tmp_line, flagi);
          }
        }
        free(tmp_line);
        tmp_line = NULL;
      }
    }
    if (exit_1 != -10) {
      if (flagi->c == 1 && flagi->l != 1) {
        if (flagi->count_f > 1 && flagi->h != 1) {
          printf("%s:", argv[flagi->count_optind]);
        }
        printf("%d\n", flagi->count_c);
      }
      if (flagi->c == 1 && flagi->l == 1) {
        if (flagi->count_c == 0) {
          if (flagi->count_f > 1 && flagi->h != 1) {
            printf("%s:", argv[flagi->count_optind]);
          }
          printf("%d\n", flagi->count_c);
        }
        if (flagi->count_c > 0) {
          if (flagi->count_f > 1 && flagi->h != 1) {
            printf("%s:", argv[flagi->count_optind]);
          }
          printf("1\n");
        }
      }
      if (flagi->l == 1 && flagi->count_l == 1) {
        printf("%s\n", argv[flagi->count_optind]);
        flagi->count_l = 0;
      }
      fclose(file);
      flagi->count_optind++;
    } else {
      flagi->count_optind = argc;
    }
  }
}

void one_argument(int argc, char *argv[], flags *flagi, char *argument) {
  FILE *file = NULL;
  char *tmp_line = NULL;
  size_t len = 0;
  regex_t regex_comp = {0};
  int line = 0;
  int cflags = 0;
  int success = 0;
  if (flagi->i == 1) {
    cflags = REG_ICASE;
  }
  regcomp(&regex_comp, argument, cflags);
  while (flagi->count_optind < argc) {
    flagi->count_l = 0;
    line = 0;
    flagi->count_c = 0;
    flagi->count_n = 1;
    if ((file = fopen(argv[flagi->count_optind], "r")) == NULL) {
      if (flagi->s == 0) {
        printf("s21_grep:%s:-no such file or directory\n",
               argv[flagi->count_optind]);
      }
    } else {
      while (line != EOF) {
        line = getline(&tmp_line, &len, file);
        if (tmp_line && line != EOF) {
          success = regexec(&regex_comp, tmp_line, 0, NULL, 0);
          output(argv, success, tmp_line, flagi);
        }
        free(tmp_line);
        tmp_line = NULL;
      }
    }
    if (flagi->c == 1 && flagi->l != 1) {
      if (flagi->count_f > 1 && flagi->h != 1) {
        printf("%s:", argv[flagi->count_optind]);
      }
      printf("%d\n", flagi->count_c);
    }
    if (flagi->c == 1 && flagi->l == 1) {
      if (flagi->count_c == 0) {
        if (flagi->count_f > 1 && flagi->h != 1) {
          printf("%s:", argv[flagi->count_optind]);
        }
        printf("%d\n", flagi->count_c);
      }
      if (flagi->count_c > 0) {
        if (flagi->count_f > 1 && flagi->h != 1) {
          printf("%s:", argv[flagi->count_optind]);
        }
        printf("1\n");
      }
    }

    if (flagi->l == 1 && flagi->count_l == 1) {
      printf("%s\n", argv[flagi->count_optind]);
      flagi->count_l = 0;
    }
    fclose(file);
    flagi->count_optind++;
  }
  free(tmp_line);
  regfree(&regex_comp);
}

int file_argument(char *tmp_line2, int *suc, char *files, flags *flagi) {
  FILE *file = NULL;
  char *tmp_line = NULL;
  int success = 1, line = 0, cflags = 0, exit_1 = 0, str = 0;
  size_t len = 0;
  regex_t regex_comp = {0};
  if (flagi->i == 1) {
    cflags = REG_ICASE;
  }
  if ((file = fopen(files, "r")) == NULL) {
    if (flagi->s == 0) {
      printf("s21_grep:%s:-no such file or directory\n", files);
      exit_1 = -10;
    }
  } else {
    while (line != EOF) {
      line = getline(&tmp_line, &len, file);
      if (tmp_line && line != EOF) {
        str = strlen(tmp_line);
        if (tmp_line[str - 1] == '\n' && str > 1) {
          tmp_line[str - 1] = '\0';
        }
        regcomp(&regex_comp, tmp_line, cflags);
        success = regexec(&regex_comp, tmp_line2, 0, NULL, 0);
        if (success == 0) {
          *suc = success;
        }
      }
      regfree(&regex_comp);
      free(tmp_line);
      tmp_line = NULL;
    }
    fclose(file);
  }
  return exit_1;
}

void output(char *argv[], int success, char *tmp_line, flags *flagi) {
  if (success == 0) {
    flagi->count_l = 1;
  }
  if (success == 0 && flagi->v == 0) {
    flagi->count_c++;
  }
  if (flagi->v == 1 && success == 1) {
    flagi->count_c++;
    flagi->count_l = 1;
    if (flagi->c == 0 && flagi->l != 1) {
      if (flagi->count_f > 1 && flagi->h != 1) {
        printf("%s:", argv[flagi->count_optind]);
      }
      if (flagi->n == 1) {
        printf("%d:", flagi->count_n);
      }
      printf("%s", tmp_line);
      if (tmp_line[strlen(tmp_line) - 1] != '\n') {
        printf("\n");
      }
    }
  }
  if (success == 0) {
    if (flagi->e == 0 && flagi->f == 0 && flagi->i == 0 && flagi->n == 0 &&
        flagi->h == 0 && flagi->s == 0 && flagi->v == 0 && flagi->c == 0 &&
        flagi->l == 0) {
      if (flagi->count_f > 1) {
        printf("%s:", argv[flagi->count_optind]);
      }
      printf("%s", tmp_line);
    }
  }
  if ((flagi->e == 1 || flagi->f == 1 || flagi->i == 1 || flagi->n == 1 ||
       flagi->h == 1 || flagi->s == 1) &&
      success == 0) {
    if (flagi->c == 0 && flagi->v != 1 && flagi->l != 1) {
      if (flagi->count_f > 1 && flagi->h != 1) {
        printf("%s:", argv[flagi->count_optind]);
      }
      if (flagi->n == 1) {
        printf("%d:", flagi->count_n);
      }
      printf("%s", tmp_line);
      if (tmp_line[strlen(tmp_line) - 1] != '\n') {
        printf("\n");
      }
    }
  }
  flagi->count_n++;
}
