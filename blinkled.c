#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

  FILE* pinMode = fopen(fichier,"w");
  if(pinMode!=NULL){
    fprintf(pinMode,"%s",mode);
    fclose(pinMode);
  }
  else{printf("pinMode failed\n");}
}


void main(void){
  char* pinName;
  char* mode;
  char fichier[100];
  
  FILE* ioval;
  pinName = "10";
  mode = "out";

  snprintf(fichier, sizeof fichier, "/sys/class/gpio/gpio%s/value", pinName);
  setGPIO(pinName,mode);
  ioval = fopen(fichier, "w");

  while(1){;

    if(ioval!=NULL){

      fprintf(ioval,"%d",1);
      sleep(2);
      printf("1\n");
      fflush(ioval);
      fprintf(ioval,"%d",0);
      sleep(2);
      printf("0\n");
      fflush(ioval);
    }
    else{printf("value failed\n");}
  }
  fclose(ioval);
}
