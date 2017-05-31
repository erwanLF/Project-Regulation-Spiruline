/*Init protocole:
take the dts file : http://klaus.ede.hih.au.dk/index.php/BBB_and_OneWire
compile the device tree :
sudo wget -c https://raw.githubusercontent.com/RobertCNelson/tools/master/pkgs/dtc.sh
sudo chmod u+x dtc.sh
sudo ./dtc.sh
dtc -O dtb -o w1-00A0.dtbo -b 0 -@ 1wire.dts
mw w1-00A0;dtbo /lib/firmware
Activate the one wire
sudo echo w1 > /sys/devices/bone.capemgr.9/slots
to verify dmesg or cat /sys/devices/bone_capemgr.9/slots
check the value sudo cat /sys/devices/w1_bus_master1/29-/w1-slave*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define  LINE_MAXLEN  1000

//putty 192.168.0.26
//res 4.7k  in pull-up mode
//VCC 3V
//data P8_11

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
  //     // get line in <buf> then search in it substring "t⁼"

  if(ferror(FileValue)) {
    // error management
    return EXIT_FAILURE;
  }

  double val = strtod(subbuf+2, NULL);  // +2 to skip 't' and '=' characters

  if(errno == ERANGE) {
    // if the correct value cause overflow
    return EXIT_FAILURE;
  }
  return val;
}

int main(int argc, char const *argv[]) {
  double WaterTemp;
  init_DS18b20();
  while(1){
    WaterTemp=getWaterTemp();
    printf("Temperature eau  : %.3lf^C\n", WaterTemp*0.001);
  }
  return 0;
}
