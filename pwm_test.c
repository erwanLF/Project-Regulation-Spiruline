#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void main(void){

	while(1){
		FILE* f1 = fopen("/sys/devices/ocp.3/pwm_test_P9_14.15/period","w+");
		//printf("f1 open\n");
		if(f1!=NULL){
			fprintf(f1,"10030000");
			//printf("f1 write\n");
			fclose(f1);
		}
		else{printf("f1 couldn't be opened\n");}
		f1 = fopen("/sys/devices/ocp.3/pwm_test_P9_14.15/duty","w+");
		if(f1!=NULL){
                        fprintf(f1,"10000000");
                        //printf("f2 write\n");
                        fclose(f1);
                }
                else{printf("f2 couldn't be opened\n");}

	}

}
