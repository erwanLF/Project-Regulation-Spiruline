//DSB18B20 //res 4.7k in pull-up mode
//VCC 3V
//data P8_11

//DHT22
//VCC 3V
//data P8_12
#ifndef MANIP_H
#define MANIP_H

#include "../dht22/bbb_dht_read.h"
#include <stdio.h>
#include "../tsl2561.h"
#include "../moisture.h"
#include <string.h>
#include <errno.h>
#include "../setGPIO.h"

#define LINE_MAXLEN  1000



void init_DS18b20(){
  FILE* Onewire;
  Onewire = fopen("/sys/devices/bone_capemgr.9/slots","w");
  if (Onewire!=NULL) {
    fprintf(Onewire,"w1");
    fclose(Onewire);
  }
  else{printf("Onewire setting failed\n");}
}

double getWaterTemp(){
  FILE* FileValue;
  int tab[1000];
  int c,index;
  char buf[LINE_MAXLEN];
  char * subbuf;
    index=0;
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

#endif
