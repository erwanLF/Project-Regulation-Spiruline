#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int Init_i2c(){
        int file;
         // Create I2C bus
        char *bus = "/dev/i2c-1"; // use  i2cdetect -y -r *
        if((file = open(bus, O_RDWR)) < 0){ // read and write mode
                printf("Failed to open the bus. \n");
                exit(1);
        }
        // Get I2C device, TSL2561 I2C address is 0x29
        ioctl(file, I2C_SLAVE, 0x29);

        // Select control register(0x00 | 0x80)
        // Power ON mode(0x03) figure 4
        char config[2] = {0};
        config[0] = 0x00 | 0x80;
        config[1] = 0x03;
        write(file, config, 2);
        // Select timing register(0x01 | 0x80)
        // Nominal integration time = 402ms(0x02) figure 6
        config[0] = 0x01 | 0x80;
        config[1] = 0x02;
        write(file, config, 2);
        sleep(1);
        // Reacd 4 bytes of data from register(0x0C | 0x80)
        // ch0 lsb, ch0 msb, ch1 lsb, ch1 msb
        char reg[1] = {0x0C | 0x80};
        write(file, reg, 1);
        return file;
}
