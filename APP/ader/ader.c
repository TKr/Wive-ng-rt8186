#include <stdlib.h>
#include <stdio.h>
int main(int argc, char **argv){
  if(argc<3){
    fprintf(stderr, "ader {char num}\n");
    exit(2);
  }
  //параметры
  char *str=argv[1];
  int num=atoi(argv[2]);
  int a;
  for(a=0; a<num; a++)
    printf("%s", str);
  
  return 0;
}
