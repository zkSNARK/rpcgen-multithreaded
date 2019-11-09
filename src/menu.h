#pragma once


/////////////////////////////////////////////////
#include <ctype.h>

char buf[80];

int nextin(char *cmd, char *word) {
  int i, ch;
  printf("\n");
  printf("***** Make a choice ******\n");
  printf("1. I(initialize dictionary)\n");
  printf("2. i(inserting word) \n");
  printf("3. l(looking for word)\n");
  printf("4. d(deleting word)\n");
  printf("5. q(quit)\n");
  printf("***************************\n");
  printf("Command prompt => \t");
  ch = getc(stdin);
  while (isspace(ch))
    ch = getc(stdin);
  if (ch == EOF)
    return -1;
  *cmd = (char)ch;
  sprintf(buf, "%s", cmd);
  if (buf[0] == 'q' || buf[0] == 'I')
    return 0;
  printf("*****************\n");
  printf("Analysing Command\n");
  printf("*****************\n");
  if (buf[0] == 'i' || buf[0] == 'l' || buf[0] == 'd') {
    printf("Input word  => \t");
  } else {
    return 0;
  }
  ch = getc(stdin);
  while (isspace(ch))
    ch = getc(stdin);
  if (ch == EOF)
    return -1;
  if (ch == '\n')
    return 0;
  i = 0;
  while (!isspace(ch)) {
    if (++i > MAXWORD) {
      printf("Error word to long.\n");
      exit(1);
    }
    *word++ = ch;
    ch = getc(stdin);
  }
  return i;
}
/////////////////////////////////////////////////