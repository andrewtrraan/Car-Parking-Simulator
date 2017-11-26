#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include <ugpio/ugpio.h>
#define MAXCHAR 1000
int main() {






/*	gpio_request(3,NULL);	
	gpio_direction_output(3,0);

	printf("test 1:\n");
	
	int push=0;
	
	for(int i=0 ; i<10 ; i++){
		
		//push = gpio_get_value(1);
		//printf("push is: %d\n",push);
		printf("high");
		gpio_set_value(3,1);
		sleep(2);
		//push = gpio_get_value(0);
		//printf("push is: %d\n",push);
		printf("low");
		gpio_set_value(3,0);
		sleep(2);
		
	}
	gpio_free(3);*/


	

    FILE *fp;
    char str[MAXCHAR];
    char* filename = "/dev/ttyS1";
 	float theDistance =0;
    fp = fopen(filename, "r");
    if (fp == NULL){
        printf("Could not open file %s",filename);
        return 1;
    }
    while (fgets(str, MAXCHAR, fp) != NULL){
	theDistance = atof(str);
	gpio_request(3,NULL);
	gpio_request(2,NULL);
	gpio_direction_output(2,0);
	gpio_direction_output(3,0);

	if(theDistance<100){
        	printf("%s", str);
		//turns green led on
		gpio_set_value(3,1);
		//sleep(1);
		
		//turns red led off
		gpio_set_value(2,0);
		sleep(1);
	}
	else{
		printf("cheesecake");
		//turns green led off
		gpio_set_value(3,0);
		
		
		//turns red led on
		gpio_set_value(2,1);
		sleep(1);
	}
	gpio_free(3);
	gpio_free(2);
}
    fclose(fp);

    return 0;
}
