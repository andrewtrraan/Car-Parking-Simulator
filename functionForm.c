#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
//#include "curlver.h"

#include <ugpio/ugpio.h>
#include <time.h>
#define MAXCHAR 1000

void callEmail(){
    char buff[20];
    struct tm *sTm;
    time_t now = time (0);
    sTm = localtime (&now);
    strftime (buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", sTm);
    //printf(buff);

    // Call the Python script passing a filename argument.
    char* command = "mailsend -f garagesensor150@gmail.com -smtp smtp.gmail.com -t abdum.shk@gmail.com -sub \"Your car has entered the garage\" -cs \"us-ascii\" -enc-type \"7bit\" -M " + \"Car entered the garage at\" + buff +  \"This is a test\" -starttls -user garagesensor150@gmail.com -port 587 -auth";
	  system(command);
}

bool checkDistance(float theDistance){

	gpio_request(3,NULL);
	gpio_request(2,NULL);
	gpio_request(8,NULL);
	gpio_direction_output(2,0);
	gpio_direction_output(3,0);
	gpio_direction_output(8,0);
	/*if(theDistance>2&& leftDistance ==true){

        	printf("%s", str);
		//turns green led on
		gpio_set_value(3,1);
		//sleep(1);
		
		//turns red led off
		gpio_set_value(2,0);
		gpio_set_value(8,1);
		sleep(1);
		printf("parking success");
		rightDistance=true;

	}*/
	 if(theDistance<10&&theDistance>2){
		//goodDistance=true;

		printf("parking success");


        	printf("%9.6f", theDistance);
		//turns green led on
		gpio_set_value(3,1);
		//sleep(1);
		
		//turns red led off
		gpio_set_value(2,0);
		gpio_set_value(8,1);
		sleep(1);
		return false;
		


	}
	/*else if(theDistance >2&&goodDistance ==true){
		//printf("cheesecake");
		//turns green led off
		gpio_set_value(3,0);
		
		
		//turns red led on
		gpio_set_value(2,1);
		leftDistance ==true;
		sleep(1);
	}*/


	/*else if(theDistance<10&&theDistance>0){
     		printf("%s", str);
		//printf("cheesecake");

		//turns green led off
		gpio_set_value(3,0);
		
		//printf("good distance");
		//turns red led on
		gpio_set_value(2,1);

		sleep(1);
		//goodDistance ==true;
	}*/
	else if(theDistance>11&&theDistance<100){
		printf("%9.6f", theDistance);
		//turns green led off
		gpio_set_value(3,0);
		
		
		//turns red led on
		gpio_set_value(2,1);
		sleep(1);
	}
	gpio_free(3);
	gpio_free(2);
	gpio_free(8);
	return true;
}
int readFile(char*theFile){
	

    FILE *fp;
    char str[MAXCHAR];

 	float theDistance =0;
    fp = fopen(theFile, "r");
    if (fp == NULL){
        printf("Could not open file %s",theFile);
        return 1;
    }

    while ((fgets(str, MAXCHAR, fp) != NULL)){
	theDistance = atof(str);

	
	if(checkDistance(theDistance)==false){
		callEmail();
		break;
	}
	
}
    fclose(fp);
	return 0;
}
	
int main() {

    char* filename = "/dev/ttyS1";
	readFile(filename);
    return 0;
}
