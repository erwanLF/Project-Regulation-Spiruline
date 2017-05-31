#ifndef _SETGPIO_H_
#define _SETGPIO_H_

#include <stdio.h>
#include <stdlib.h>

void setGPIO(char* pinName, char* mode){
  printf("pinName=%s & mode=%s\n",pinName,mode);
  char fichier[100];
  snprintf(fichier, sizeof fichier, "/sys/class/gpio/gpio%s/direction", pinName);
  puts(fichier);
  FILE *pinSet = fopen("/sys/class/gpio/export","w");
  if(pinSet!=NULL){
    fprintf(pinSet,"%s",pinName);
    fclose(pinSet);
  }
  else{printf("pinSet failed\n");}
  //FILE* pinMode = fopen(buf,"w");
  FILE* pinMode = fopen(fichier,"w");
  if(pinMode!=NULL){
    fprintf(pinMode,"%s",mode);
    fclose(pinMode);
  }
  else{printf("pinMode failed\n");}
}

#endif
