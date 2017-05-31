//DSB18B20 //res 4.7k in pull-up mode
//VCC 3V
//data P8_11

//DHT22
//VCC 3V
//data P8_12

#include "bbb_dht_read.h"
#include <stdio.h>
#include "../tsl2561.h"
#include "../moisture.h"
#include <string.h>
#include <errno.h>
#include "setGPIO.h"

#define LINE_MAXLEN  1000


void init_DS18b20();
//void initConductivite();
double getWaterTemp();


int main(int argc, char const *argv[]) {
  float humidity;
  float temperature;
  int gpio_base=1;
  int gpio_number=12;
  int check;
  double salinity;
  double WaterTemp;

  FILE* Onewire;
  FILE* FileValue;
  int tab[1000];
  int c,j,index;
  float temp;
  int flotter_top;
  int flotter_bottom;
  float waterReal;
  int file,i=0;
  char data[4]={0};
  int tablo[4];
  int tab2[4];
  //printf("before function");
  file=Init_i2c();
  //printf("Init_i2c");
  FILE* fichier= NULL;
  fichier = fopen("data.txt","r+");

  FILE* ioval;
  FILE* ioval2;
  FILE* ioval3;
  FILE* ioval4;
  FILE* ioval5;
  FILE* f;
  FILE* g;

  char* pinName_flotter_low;
  char* pinName_flotter_high;
  char* pinName_LedWaterMotor;
  char* pinName_LedpHMotor;
  char* pinName_LedOver;
  char* pinName_LedSalinity;
  char* pinName_Ledfan;
  char* mode= "out";
  char* mode2= "in";
  char file_flotter_low[100];
  char file_flotter_high[100];
  char fichier_LedWaterMotor[100];
  char fichier_LedpHMotor[100];
  char fichier_LedOver[100];
  char fichier_LedSalinity[100];
  char fichier_Ledfan[100];
  char buffer[BUFFER_SIZE];

  pinName_flotter_low     = "47";
  pinName_flotter_high    = "46";
  pinName_LedWaterMotor   = "117";
  pinName_LedpHMotor      = "86";
  pinName_LedOver         = "14";
  pinName_LedSalinity     = "87";
  pinName_Ledfan          = "10";


  snprintf(fichier_LedWaterMotor, sizeof fichier_LedWaterMotor, "/sys/class/gpio/gpio%s/value", pinName_LedWaterMotor);
  snprintf(fichier_LedpHMotor, sizeof fichier_LedpHMotor, "/sys/class/gpio/gpio%s/value", pinName_LedWaterMotor);
  snprintf(fichier_LedOver, sizeof fichier_LedOver, "/sys/class/gpio/gpio%s/value", pinName_LedOver);
  snprintf(fichier_LedSalinity, sizeof fichier_LedSalinity, "/sys/class/gpio/gpio%s/value", pinName_LedSalinity);
  snprintf(fichier_Ledfan, sizeof fichier_Ledfan, "/sys/class/gpio/gpio%s/value", pinName_Ledfan);


  snprintf(file_flotter_low, sizeof file_flotter_low, "/sys/class/gpio/gpio%s/value", pinName_flotter_low);
  snprintf(file_flotter_high, sizeof file_flotter_high, "/sys/class/gpio/gpio%s/value", pinName_flotter_high);

  setGPIO(pinName_LedWaterMotor,mode);
  setGPIO(pinName_LedpHMotor,mode);
  setGPIO(pinName_LedOver,mode);
  setGPIO(pinName_LedSalinity,mode);
  setGPIO(pinName_Ledfan,mode);

  setGPIO(pinName_flotter_high,mode2);
  setGPIO(pinName_flotter_low,mode2);

  ioval  = fopen(fichier_LedWaterMotor, "w");
  ioval2 = fopen(fichier_LedpHMotor, "w");
  ioval3 = fopen(fichier_LedOver, "w");
  ioval4 = fopen(fichier_LedSalinity, "w");
  ioval5 = fopen(fichier_Ledfan, "w");

   // f = fopen(file_flotter_low,"r");
    //g = fopen(file_flotter_high, "r");

  if(ioval!=NULL){fprintf(ioval,"%d",0);}else{printf("NON 2\n");}
  if(ioval2!=NULL){fprintf(ioval2,"%d",0);}else{printf("NON 2\n");}
  if(ioval3!=NULL){fprintf(ioval3,"%d",0);}else{printf("NON 3\n");}
  if(ioval4!=NULL){fprintf(ioval4,"%d",0);}else{printf("NON 4\n");}
  if(ioval5!=NULL){fprintf(ioval5,"%d",0);}else{printf("NON 5\n");}

  initConductivite();
  init_DS18b20();

while(1){
    f = fopen(file_flotter_low,"r");
    g = fopen(file_flotter_high, "r");
  j=0;
  index=0;

    flotter_bottom=fgetc(f);
    flotter_top=fgetc(g);

  if(flotter_bottom == '0' && flotter_top == '0'){
    fprintf(ioval,"%d",1);
    fprintf(ioval3,"%d",0);
  }
  else if(flotter_bottom =='1' && flotter_top == '0' ){
    fprintf(ioval3,"%d",0);
  }
  else if(flotter_bottom == '1' && flotter_top == '1' ){
    fprintf(ioval,"%d",0);
    fprintf(ioval3,"%d",1);
  }
  else{
    printf("impossible !!!\n");
    exit(0);
  }
  fflush(ioval);
  fflush(ioval3);
  index=0;
  check=bbb_dht_read(DHT22,gpio_base,gpio_number,&humidity,&temperature);

  if (check==0) {
    printf("Hum=%.3f%%----->Temp=%.3f^C\n",humidity,temperature);
  }
  WaterTemp=getWaterTemp();
  printf("Temperature eau  : %.3lf^C\n", WaterTemp*0.001);
  sleep(1);

  if(read(file, data, 4) != 4){
          printf("Error : Input/output Error \n");
  }
  else{
          if(fichier != NULL){
            // Convert the data
            float ch0 = (data[1] * 256 + data[0]);
            float ch1 = (data[3] * 256 + data[2]);
            // Output data to screen
           printf("Full Spectrum(IR + Visible) : %.2f lux \n", ch0);
          /* printf("Infrared Value : %.2f lux \n", ch1);
           printf("Visible Value : %.2f lux \n", (ch0 - ch1));*/
           fprintf(fichier,"%.2f,%.2f,%.2f \n",ch0,ch1,ch0-ch1);
          }
          else{
            printf("Error fichier:\n");
          }
  }
  waterReal=(float)WaterTemp*0.001;

  if(waterReal>=26){
    fprintf(ioval5,"%d",1);
	sleep(1);
  }
  else{fprintf(ioval5,"%d",0);}
 fflush(ioval5);

 salinity=getSale(waterReal);
 sleep(1);

 printf("Salinity %lf\n",salinity);
 if(salinity<=25){
   fprintf(ioval4,"%d",1);
   sleep(1);
 }
 else{fprintf(ioval4,"%d",0);}

 fflush(ioval4);
 sleep(1);
}
return 0;
}




void init_DS18b20(){
  FILE* Onewire;
  Onewire = fopen("/sys/devices/bone_capemgr.9/slots","w");
  if (Onewire!=NULL) {
    fprintf(Onewire,"w1");
    fclose(Onewire);
  }
  else{printf("Onewire setting failed\n");}
  // value = "/sys/bus/w1/devices/28-00000763e764/w1_slave"
}
double getWaterTemp(){
  FILE* FileValue;
  int tab[1000];
  int c,index;
  char buf[LINE_MAXLEN];
  char * subbuf;
    index=0;
    //printf("while\n");
    FileValue=fopen("/sys/bus/w1/devices/28-00000763e764/w1_slave","r");
    while(fgets(buf, LINE_MAXLEN, FileValue) != NULL &&(subbuf = strstr(buf, "t=")) == NULL);
    // // get line in <buf> then search in it substring "tÃ¢ÂÂ¼"
    if(ferror(FileValue)) {
      // error management
      return EXIT_FAILURE;
    }
    double val = strtod(subbuf+2, NULL); // +2 to skip 't' and '=' characters
    if(errno == ERANGE) {
      // if the correct value cause overflow
      return EXIT_FAILURE;
    }
    return val;
}
