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
  char* pinName2;
  char* mode;
  char* mode2;
  char fichier[100];
  char fichier2[100];
  int flotter_low=0,flotter_high=0;

  FILE* f;
  FILE* g;
  FILE* ioval;
  pinName = "46";
  mode = "in";
  pinName2 = "47";
  mode2 = "in";

  snprintf(fichier, sizeof fichier, "/sys/class/gpio/gpio%s/value", pinName);
  snprintf(fichier2, sizeof fichier2, "/sys/class/gpio/gpio%s/value", pinName2);
 // puts(fichier);
  setGPIO(pinName,mode);
  setGPIO(pinName2,mode2);

  while(1){

    f = fopen(fichier,"r");
    g = fopen(fichier2,"r");
    flotter_low = fgetc(f);
    //sleep(1);
    flotter_high = fgetc(g);
    printf("flotter_high= %c && flotter_low= %c \n",flotter_high,flotter_low);
    sleep(1);
}
fclose(f);
fclose(g);
}
