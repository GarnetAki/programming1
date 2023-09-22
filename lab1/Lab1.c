#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int count_l = 0;
  int count_w = 0;
  int count_c = 0;
  int str;

  if (argc < 3){
    printf("Wrong number of args");
    exit(0);
  }

  FILE* fd = fopen(argv[2], "rb");
  while(getc(fd) != EOF)
		count_c++;
	fclose(fd);

  freopen(argv[2], "r", stdin);

  int flag_word = 0;
  int flag_lines = 0;
  int memory_lines = 0;
  int last = 0;

  while (1)
  {
    str = getchar();
    if (str == '\n' || str == EOF)
    {
      if (flag_lines == 1)
      {
        count_l += memory_lines;
        count_l++;
        memory_lines = 0;
        flag_lines = 0;
        last = 1;
      }
      else
      {
        memory_lines++;
        last = 0;
      }
    }
    else
    {
      flag_lines = 1;
    }
    if (str == ' ' || str == '\n' || str == EOF)
    {
      if (flag_word == 1)
      {
        count_w++;
        flag_word = 0;
      }
    }
    else
      flag_word = 1;
    if (str == EOF)
      break;
  }
  count_l += memory_lines;
  if (last == 0)
    count_l--;
  int flag = 0;
  if (count_c == 0 || count_c == -1L)
  {
    printf("Error: wrong file or file is empty");
  }
  else
  {
    if (!(strcmp(argv[1], "-l") && strcmp(argv[1], "--lines")))
    {
      printf("%d", count_l);
      flag = 1;
    }
    if (!(strcmp(argv[1], "-c") && strcmp(argv[1], "--bytes")))
    {
      printf("%d", count_c);
      flag = 1;
    }
    if (!(strcmp(argv[1], "-w") && strcmp(argv[1], "--words")))
    {
      printf("%d", count_w);
      flag = 1;
    }
    if (flag == 0)
      printf("Error: wrong first argument");
  }

  return 0;
}