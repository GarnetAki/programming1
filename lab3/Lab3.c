#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ull unsigned long long
typedef struct NODE node;

struct NODE
{
  ull data;
  ull index;
  node *next;
};

int main(int argc, char *argv[])
{
  freopen(argv[2], "r", stdin);
  FILE *fout = fopen("errors_list.txt", "w");

  char str[1000];

  ull window = 0;
  for(int i = 0; i<strlen(argv[1]); ++i){
    window *= 10;
    window+=(ull)argv[1][i]-48;    
  }

  node *null_el = (node *)malloc(sizeof(node));
  null_el->data = 0;
  null_el->index = -1;
  null_el->next = NULL;
  node *head = null_el;
  node *last = null_el;

  ull size = 0;
  ull max_size = 0;
  ull max_ind_start = 0;
  ull ind_line = 0;
  ull count_of_errors = 0;

  while (gets(str) > 0)
  {
    ind_line++;
    char c = str[0];
    int ind = 0;
    while (c != ' ')
    {
      ++ind;
      c = str[ind];
    }
    ind += 6;
    ull value = (ull)((((ull)str[ind] - 48) * 10 + ((ull)str[ind + 1] - 48)) * 24 * 3600 + 
    (((ull)str[ind + 12] - 48) * 10 + ((ull)str[ind + 13] - 48)) * 3600 + 
    (((ull)str[ind + 15] - 48) * 10 + ((ull)str[ind + 16] - 48)) * 60 + 
    ((ull)str[ind + 18] - 48) * 10 + ((ull)str[ind + 19] - 48));

    ull year = ((ull)str[ind+7]-48)*1000 + ((ull)str[ind+8]-48)*100 + ((ull)str[ind+9]-48)*10 + (ull)str[ind+10]-48;

    value += (year-1900*365+(year-1901)/4)*24*60*60;
    
    char fl = str[ind+3];
    char sl = str[ind+4];
    char tl = str[ind+5];
    ull flag = 0;
    if(year%4==0)flag=1;
    if(fl=='F'&&sl=='e'&&tl=='b')value+=31*24*3600;
    if(fl=='M'&&sl=='a'&&tl=='r')value+=(31+28+flag)*24*3600;
    if(fl=='A'&&sl=='p'&&tl=='r')value+=(2*31+28+flag)*24*3600;
    if(fl=='M'&&sl=='a'&&tl=='y')value+=(2*31+30+28+flag)*24*3600;
    if(fl=='J'&&sl=='u'&&tl=='n')value+=(3*31+30+28+flag)*24*3600;
    if(fl=='J'&&sl=='u'&&tl=='l')value+=(3*31+2*30+28+flag)*24*3600;
    if(fl=='A'&&sl=='u'&&tl=='g')value+=(4*31+2*30+28+flag)*24*3600;
    if(fl=='S'&&sl=='e'&&tl=='p')value+=(5*31+2*30+28+flag)*24*3600;
    if(fl=='O'&&sl=='c'&&tl=='t')value+=(5*31+3*30+28+flag)*24*3600;
    if(fl=='N'&&sl=='o'&&tl=='v')value+=(6*31+3*30+28+flag)*24*3600;
    if(fl=='D'&&sl=='e'&&tl=='c')value+=(6*31+4*30+28+flag)*24*3600;

    while (c != '"')
    {
      ++ind;
      c = str[ind];
    }
    ++ind;
    c = str[ind];
    while (c != '"')
    {
      ++ind;
      c = str[ind];
    }
    if(str[ind+2]=='5'){
      count_of_errors++;
      fprintf(fout, "%s\n", str);
    }

    node *new = (node *)malloc(sizeof(node));
    new->data = value;
    new->next = NULL;
    new->index = ind_line;
    last->next = new;
    last = new;
    size++;
    while(head->data==0 || value - (head->data) > window){
      size--;
      node *tmp = head;
      head = head->next;
      free(tmp);
    }
    if(size > max_size){
      max_size = size;
      max_ind_start = head->index;
    }
  }

  printf("Line start: %d \nNumber of requests: %d \nCounts of errors: %d\n", max_ind_start, max_size, count_of_errors);
  
  return 0;
}