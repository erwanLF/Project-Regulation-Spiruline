#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#define CAPE_BUS 1 // Connected to /dev/i2c-1
#define CAPE_ADDR 0x4b // HTU21D slave address

#define I2C_PATH_LEN 20 // Buffer size at least large enough to fit the max length of  "/dev/i2c-N"

int DelayTime = 500;

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

int I2C_open(uint8_t bus){
  char device[I2C_PATH_LEN];
  sprintf(device, "/dev/i2c-%d", bus);
  return open(device, O_RDWR, 0);
}

void I2C_close(int i2c_fd) {
  close(i2c_fd);
}

int I2C_setSlaveAddress(int i2c_fd, int addr) {
  int ret;
  ret = ioctl(i2c_fd, I2C_SLAVE, addr);
  if (ret < 0) return ret;
  return 0;
}


int I2C_read(int i2c_fd, void *rx_buffer, int n_bytes) {
  int ret;
  ret = read(i2c_fd, rx_buffer, n_bytes);
  if (ret < 0) return ret;
  return 0;
}

int I2C_readTransaction(int i2c_fd, uint8_t command, void *rx_buffer, int n_bytes) {
  int ret;
  ret = write(i2c_fd, &command, 1);
  if (ret < 0){ return ret;}

  ret = read(i2c_fd, rx_buffer, n_bytes);
  if (ret < 0) {return ret;}
  return 0;
}


int I2C_write(int i2c_fd, void *tx_buffer, int n_bytes) {
  int ret;
  ret = write(i2c_fd, tx_buffer, n_bytes);
  if (ret < 0){ return ret;}
  return 0;
}

int WriteByte(char Reg, char Value, int i2c_fd){
  char data[3]={0};
  data[0] = 0x01;
  data[1] = Reg;
  data[2] = Value;
  write(i2c_fd,data,3);
  // printf("Write complete\n");
}

void WriteOneWord(char Reg, short Value, int i2c_fd){
  char config[6]={0};
  config[0] = 0x01;
  config[1] = Reg;
  //config[2] = 0xe8;
  config[2] = Value &         0xFF;
  //printf("%d\n",config[2]);
  config[3] = (Value >> 8)  & 0xff;
  //printf("%d\n",config[3]);
  config[4] = (Value >> 16) & 0xff;
  //printf("%d\n",config[4]);
  config[5] = (Value >> 24) & 0xff;
  //printf("%d\n",config[5]);
  write(i2c_fd,config,6);
}


void DCMotorInit(int MotorName,char Frequency,int i2c_fd){
  // Init the DC Frequency
  //WriteOneWord(CONFIG_TB_PWM_FREQ,Frequency);
  WriteOneWord(4,Frequency,i2c_fd);
  printf("Sortie WriteWord\n");
  usleep(DelayTime);
  printf("Motorname = %d \n",MotorName);


  // Set the port as DC Motor
  if (MotorName == 1 || MotorName == 2){
    //WriteByte(TB_1A_MODE,TB_DCM);
    printf("Motorname = %d \n",MotorName);
    WriteByte(9,0,i2c_fd);
    usleep(DelayTime);
    //WriteByte(TB_1A_DIR,TB_STOP);
    WriteByte(10,3,i2c_fd);
    usleep(DelayTime);
    //WriteByte(TB_1B_MODE,TB_DCM);
    WriteByte(21,0,i2c_fd);
    usleep(DelayTime);
    //WriteByte(TB_1B_DIR,TB_STOP);
    WriteByte(22,3,i2c_fd);
    usleep(DelayTime);
  }
  if (MotorName == 3 || MotorName == 4){
    //WriteByte(TB_2A_MODE,TB_DCM);
    WriteByte(33,0,i2c_fd);
    usleep(DelayTime);
    WriteByte(34,3,i2c_fd);
    //WriteByte(TB_2A_DIR,TB_STOP);
    usleep(DelayTime);
    //WriteByte(TB_2B_MODE,TB_DCM);
    WriteByte(45,0,i2c_fd);
    usleep(DelayTime);
    //WriteByte(TB_2B_DIR,TB_STOP);
    WriteByte(46,3,i2c_fd);
    usleep(DelayTime);
  }
}
// Drive the DC Motor // Direction 1 CW | 2 CCW // PW1NDuty 0 ~ 100
void DCMotorMove(int MotorName,char Direction,char PWMDuty,int i2c_fd){
  if (MotorName == 1){
    //WriteByte(TB_1B_DIR,Direction);
    WriteByte(22,Direction,i2c_fd);
    usleep(DelayTime);
    //WriteOneWord(TB_1B_DUTY,PWMDuty*10);
    WriteOneWord(23,PWMDuty*10,i2c_fd);
    usleep(DelayTime);

  }
  if (MotorName == 2){
    //WriteByte(TB_1A_DIR,Direction);

    WriteByte(10,Direction,i2c_fd);
    usleep(DelayTime);
    //WriteOneWord(TB_1A_DUTY,PWMDuty*10);
    WriteOneWord(11,PWMDuty*10,i2c_fd);

    usleep(DelayTime);
  }
  if (MotorName == 3){
    //WriteByte(TB_2B_DIR,Direction);
    WriteByte(46,Direction,i2c_fd);
    usleep(DelayTime);
    //WriteOneWord(TB_2B_DUTY,PWMDuty*10);
    WriteOneWord(47,PWMDuty*10,i2c_fd);
    usleep(DelayTime);
  }
  if (MotorName == 4){
    //WriteByte(TB_2A_DIR,Direction);
    WriteByte(34,Direction,i2c_fd);
    usleep(DelayTime);
    //WriteOneWord(TB_2A_DUTY,PWMDuty*10);
    WriteOneWord(35,PWMDuty*10,i2c_fd);
    usleep(DelayTime);
  }
}

// Stop the DC motor
void DCMotorStop(int MotorName,int i2c_fd){
  if (MotorName == 1){
    //WriteByte(TB_1B_DIR,TB_STOP);
    WriteByte(22,3,i2c_fd);
  }
  if (MotorName == 2){
    //WriteByte(TB_1A_DIR,TB_STOP);
    WriteByte(10,3,i2c_fd);

  }
  if (MotorName == 3){
    //WriteByte(TB_2B_DIR,TB_STOP);
    WriteByte(46,0,i2c_fd);

  }
  if (MotorName == 4 ){
    //WriteByte(TB_2A_DIR,TB_STOP);
    WriteByte(34,3,i2c_fd);

  }
  usleep(DelayTime);
}

int main(int argc, char const *argv[]) {
  int i2c_fd,slave;
  int MotorName = 1;
  int MotorName2 = 2;
  char ClockWise = 1;
  char CounterClockWise = 2;
  char PwmDuty = 100;
  short Frequency = 1000;
  //open i2c device
  char *bus = "/dev/i2c-1"; // use  i2cdetect -y -r *
  if((i2c_fd = open(bus, O_RDWR)) < 0){ // read and write mode
    printf("Failed to open the bus. \n");
  }
  else{printf("OPen\n");}
  printf("DC Motor Init -->\n");
  // slave=I2C_setSlaveAddress(i2c_fd,0x4b);
  if (ioctl(i2c_fd, I2C_SLAVE, 0x4b)<0){

    printf("I2C slave Word  failed\n");
  }
  
  DCMotorInit(MotorName2,Frequency,i2c_fd);

  while(1){
    // DCMotorInit(MotorName,Frequency,i2c_fd);
    sleep(1);
    DCMotorMove(MotorName2,CounterClockWise,PwmDuty,i2c_fd);
    sleep(1);
    /* DCMotorStop(MotorName,i2c_fd);
    sleep(3);
    DCMotorInit(MotorName2,Frequency,i2c_fd);
    sleep(1);
    DCMotorMove(MotorName2,CounterClockWise,PwmDuty,i2c_fd);
    sleep(1);
    DCMotorStop(MotorName2,i2c_fd);
    */
  }
  I2C_close(i2c_fd);
  return 0;
}
