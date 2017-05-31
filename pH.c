/*
  Sonde pH SEN0169
  La sonde de pH est analogique
  La sonde ressort du 5V
  Alors que les entrée analogique ne supporte que du 3V3
  Il faut utiliser un pont diviseur de tension

*/


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 128

void init_pH(){
 char buffer[BUFFER_SIZE];
 int num  = 2;
 FILE *fichier=fopen("/sys/devices/bone_capemgr.9/slots","w");
 if(fichier!=NULL){
   fprintf(fichier,"cape-bone-iio");
   fclose(fichier);
 }

 snprintf(buffer, BUFFER_SIZE, "/sys/devices/ocp.3/helper.12/AIN%d",num);

}

float getPh(FILE *f){
 char buffer[BUFFER_SIZE];
 float res;
 int nombre = 10;
 f = fopen(buffer,"r");
 res = nombre*0.071-0.47;
 printf("Brut = %d    pH = %f\n",nombre,res);
 return res;
}

void closeFile(FILE* f){
 fclose(f);
}

int main(void){

FILE* f;
float Ph;
int nombre =  10;

 init_pH();

 while(1){
 Ph  = getPh(f);
 printf("pH = %f\n",Ph);
 }
closeFile(f);
}
