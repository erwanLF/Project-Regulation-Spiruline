
//Moisture sensre
// Analog devices
//enable Analog : sudo echo cape-bone-iio > /sys/devices/bone_capemgr.9/slots
//Place to see value: ls -l /sys/devices/ocp.3/helper.12/
// to see value : sudo cat /sys/devices/ocp.3/helper.12/AIN0
// equation salinity in this site : http://envlit.ifremer.fr/var/envlit/storage/documents/dossiers/prelevementhydro/chapitre8.html

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define A0     0.0080
#define A1    -0.01692
#define A2     25.3851
#define A3     14.0941
#define A4    -7.0261
#define A5     2.7081
#define sumA  A0+A1+A2+A4+A5

#define B0     0.0005
#define B1    -0.0056
#define B2    -0.0066
#define B3    -0.0375
#define B4     0.0636
#define B5    -0.0144
#define sumB  B0+B1+B2+B3+B4+B5

#define C0     0.6766097
#define C1     0.0200564
#define C2     0.00011043
#define C3     -0.00000069698
#define C4     0.0000000010031

#define K       0.0162

#define BUFFER_SIZE 128



void init_Moisture(){
  char buffer[BUFFER_SIZE];
  int num  = 0; //Analog pin
  FILE *fichier=fopen("/sys/devices/bone_capemgr.9/slots","w");
  if(fichier!=NULL){
    fprintf(fichier,"cape-bone-iio");
    fclose(fichier);
  }
  snprintf(buffer, BUFFER_SIZE, "/sys/devices/ocp.3/helper.12/AIN%d",num);
}


int main(int argc, char const *argv[]) {
  double  S,R,rt,Rt;
  int i,temperature,Capteur_conductivite;
  char buffer[BUFFER_SIZE];
  float Conductivite;
  int num  = 0; //Analog pin number
  int temp,temp2,temp3;
  int tab[4];
  FILE *fichier=fopen("/sys/devices/bone_capemgr.9/slots","w");
  if(fichier!=NULL){
    fprintf(fichier,"cape-bone-iio");
    fclose(fichier);
  }
  snprintf(buffer, BUFFER_SIZE, "/sys/devices/ocp.3/helper.12/AIN%d",num);

  FILE* f = fopen(buffer,"r");
  i=0;
  do{
    tab[i]=fgetc(f);
    if(tab[i]!=10 &&  tab[i]>0){
      printf("%c\n",tab[i]);
    }
    i++;
  } while(i != 3);

  if(i==3){
    Capteur_conductivite=(tab[0]-48)*100+(tab[1]-48)*10+tab[2]-48;
  }
  else if(i==2){
    Capteur_conductivite=(tab[0]-48)*10+tab[1]-48;
  }
  else{
    Capteur_conductivite=tab[0]-48;
  }

  printf("%d\n",Capteur_conductivite);

  temperature=22;
  rt = C0 + C1*temperature + C2*pow(temperature,2) + C3*pow(temperature,3) + C4*pow(temperature,4);

  // Approximative equation due to the hazardous data of Moisture sensor
  if(Capteur_conductivite<956){
    Conductivite= 0.078*Capteur_conductivite-29.1135;
  }

  else{Conductivite=18.5902*Capteur_conductivite-17727;}
  //printf("Conductivity = %lf\n",Conductivite);

  Rt = Conductivite/(42.914*rt);
  printf("Rt= %lf\n",Rt);
  R  = Conductivite/42.914;
  double  dRt = Rt*Rt;
  S = A0 + A2*Rt+ A1*sqrt(Rt) + A3*sqrt(Rt)*Rt + A4*pow(Rt,2) + A5*pow(Rt,2)*sqrt(Rt) + ((temperature-15)/(1+K*(temperature-15)))*(B0 + B1*sqrt(Rt)+B2*Rt+B3*pow(Rt,1.5)+B4*pow(Rt,2)+B5*pow(Rt,2.5));
  printf("Pour une T=%d et une conductivité de :%f  on a la saliné suivante  %lf \n",temperature,Conductivite,S);
  return 0;
}
