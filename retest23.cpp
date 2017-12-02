//#include <iostream>
//#include <iostream>

//#include <iostream>
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <string>
#include <fstream>
#include <unistd.h>
//#include <fcntl.h>
#include <stdbool.h>
//#include <vector>
//
//#include "curlver.h"
#include <math.h>
#include <ugpio/ugpio.h>
#include <time.h>
#define MAXCHAR 1000
#define vectorSize =1000;
//num days is the total number of distances
using namespace std;

struct DayCar{ //struct for calculating statistics for 1 day
	int garageEnEx; // number of times car has entered and exited the garage in one day
};
struct Dataset{
	int numDays;
	DayCar* car;
};
struct Rejects{
	int numRejects;
	int* rejects;
};

struct Mode{
	int numModes;
	int* modes;
};

struct Statistics{
	int   minimum;
	float average;
	int   maximum;
	float popStdDev;
	float smplStdDev;
	Mode  mode;
	int   histogram[10];
};

void selection(int dataset[], const int size, int startIndex){
	if ( startIndex >= size - 1 )
        return;
    int minIndex = startIndex;
    for ( int index = startIndex + 1; index < size; index++ )
    {
        if (dataset[index] < dataset[minIndex] )
            minIndex = index;
    }
    int temp = dataset[startIndex];
    dataset[startIndex] = dataset[minIndex];
    dataset[minIndex] = temp;
    selection(dataset, size, startIndex + 1);
}

bool sort(int dataset[], const int size){
	if(size == 0)
		return false;

	selection(dataset, size, 0);
	return true;
}

float max(Dataset& data){
	float max = data.car[0].garageEnEx;
	for(int i = 1; i < data.numDays; i++){
		float val = data.car[i].garageEnEx;
		if(val >= max){
			max = val;
		}
	}
	return  max;
}

float average(Dataset& data){
	float sum = 0;
	for(int i = 0; i < data.numDays; i++){
		sum += data.car[i].garageEnEx;
	}
	return sum/data.numDays;
}

float popStd(Dataset& data, float avg){
		//x subtract x bar
		float xSxBar =0;
		int x =0;
			//total x subtract xbar
		float xSxBarF =0;
		//squaring the x minus x bar
		float xSquared =0;
		while(x<data.numDays)
			{
					float currY= data.car[x].garageEnEx;
					xSxBar= currY - avg;
					//cout<< xSxBar<<endl;
					xSquared =xSxBar*xSxBar;
					xSxBarF+=xSquared;
					x+=1;
					//	cout<< avg<<endl;
			}
		float theDivider = x;
		return sqrt((1/theDivider)*(xSxBarF));

}
float smpStd(Dataset& data, float avg){
	float sum = 0;
	for(int j = 0; j < data.numDays; j++){
		float power = pow((data.car[j].garageEnEx	- avg),2);
		sum += power;
	}
	// if(data.numDays == 1)
	// 	//return std::numeric_limits<float>::infinity();
	// else
		return sqrt((sum/(data.numDays - 1)));
}
int theMode(Dataset& data, Statistics stats, Mode& mode){
	int i = 0;
	int largestMode = 1;
	int tempMode;
	mode.modes = new int[data.numDays];
	mode.numModes =-1;
	while(i < data.numDays){
		tempMode = 1;
		for(int j = i + 1; j < data.numDays; j++){
			if(data.car[j].garageEnEx == data.car[i].garageEnEx){
				tempMode++;
			}
		}
		if(tempMode > largestMode)
			largestMode = tempMode;
		while(data.car[i].garageEnEx == data.car[i+1].garageEnEx){
			if(i > data.numDays)
				break;
			i++;
		}
		if(data.car[i].garageEnEx != data.car[i+1].garageEnEx)
			i++;
	}
	int a = 0;
	i = 0;
	int numModes = 0;
	while(i < data.numDays){
		tempMode = 1;
		for(int j = i + 1; j < data.numDays; j++){
			if(data.car[j].garageEnEx == data.car[i].garageEnEx){
				tempMode++;
			}
		}
		if(tempMode == largestMode){
			mode.modes[a] = data.car[i].garageEnEx;
			numModes++;
			a++;
		}

		while(data.car[i].garageEnEx == data.car[i+1].garageEnEx){
			if(i > data.numDays)
				break;
			i++;
		}
		if(data.car[i].garageEnEx != data.car[i+1].garageEnEx)
			i++;
	}
	mode.numModes = numModes;
	return numModes;
}


const char * timestamp()
{
	time_t ltime; /* calendar time */
	ltime=time(NULL); /* get current cal time */
	printf("%s",asctime( localtime(&ltime) ) );
	return asctime(localtime(&ltime));
}
int writeTheData(const char filename[],int data[],int counter){
	FILE *theData = NULL;
	theData = fopen(filename, "a");
	if (theData == NULL)
    {
        printf("Error! can't open log file.");
        return -1;
    }


    fprintf(theData, "%d", data[counter]);
    fclose(theData);
		return 0;
}
int readCSV(const char filename[],Dataset& data){
	if(filename == NULL){
		return -1;
}
	data.car = new DayCar[100];
	data.numDays = 0;
	int k = 0; //index for student array
	const int maxLineLength = 100;
	char line[maxLineLength];
	FILE *input = NULL;
	input = fopen(filename, "r");
	if (input == NULL)
    {
        printf("Error! can't open log file.");
        return -1;
    }
	bool done = false;
	int fileLineNumber = 0;
	while (fgets(line, 100, input)) { // Read from file
		++fileLineNumber;
		int i = 0;
		int numEnEx = 0; //track number of enter/exits
		while(line[i] != '\n'){
			if(line[i] >= '0' && line[i] <= '9'){
				numEnEx = numEnEx * 10 + line[i] - 48;
				i++;
			}
			else if(line[i] < '0' || line[i] > '9'){
				return -1;
			}
			else{
				return -1;
			}
		}
		data.car[k].garageEnEx = numEnEx;
		data.numDays++;
		k++;
	}
	fclose(input);
return 0;
}

int computeStatistics(Dataset& data, Statistics& stats){
	if(data.numDays <=0){
		return -1;
	}
	int arr[data.numDays];
	for(int i = 0; i < data.numDays; i++){
		arr[i] = data.car[i].garageEnEx;
	}
	bool call = sort(arr, data.numDays);

	stats.minimum = arr[0];
	stats.maximum = arr[data.numDays - 1];
	stats.average = average(data);
	stats.popStdDev = popStd(data, stats.average);
	stats.smplStdDev = smpStd(data, stats.average);
	theMode(data, stats, stats.mode);
	if(stats.histogram==NULL){
				return -1;
	}
	int x = 0;
	while(x < data.numDays){
		int bucket = data.car[x].garageEnEx/10;
				//cout<<bucket<<endl;
		stats.histogram[bucket]+=1;
		x++;
	}
	return 0;
}
int writeLog(const char filename[], const char severity[], const char description[]){
	FILE *log = NULL;
	log = fopen(filename, "a");
	if (log == NULL)
    {
        printf("Error! can't open log file.");
        return -1;
    }


    fprintf(log, "%s%s %s\n", timestamp(), severity, description);
    fclose(log);
		return 0;
}

void callEmail(){
    char buff[20];
    struct tm *sTm;
    time_t now = time (0);
    sTm = localtime (&now);
   strftime (buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", sTm);
    //cout << buff << endl;

    // Call the Python script passing a filename argument.
    char* command = "mailsend -f garagesensor150@gmail.com -smtp smtp.gmail.com -t abdum.shk@gmail.com -sub \"testing oneline messages\" -cs \"us-ascii\" -enc-type \"7bit\" -M \"This is a test\" -starttls -user garagesensor150@gmail.com -port 587 -auth";
	//char* command 	
	system(command);
}
bool checkDistance(int theDistance,int &theCounter, int theArray[]){
	//need an array and a counter
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
		writeLog("log.txt","INFO", "Parking success");
		writeLog("log.txt","INFO", "Green LED turning on");
		writeLog("log.txt","INFO", "Red LED turning off");
		
		char str[5];
        	sprintf(str,"%d", theDistance);
		writeLog("log.txt", "INFO", str);
		//std::cout<<theDistance<<endl;
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
		//printf("%9.6f", theDistance);
		//cout<<theDistance<<endl;
		//turns green led off
		writeLog("log.txt","INFO", "Parking not successful yet");
		writeLog("log.txt","INFO", "Red LED turning on");
		writeLog("log.txt","INFO", "Green LED turning off");
		gpio_set_value(3,0);
		
		
		//turns red led on
		gpio_set_value(2,1);
		sleep(1);
	}
	theArray[theCounter] =theDistance;
	writeTheData("theData.txt",theArray,theCounter);

	gpio_free(3);
	gpio_free(2);
	gpio_free(8);
	return true;
}
int readFile(char*theFile){
	

    FILE *fp;
    char str[MAXCHAR];

 	int theDistance =0;
	int counter =0;
	int arrayOfDistances[1000];
    fp = fopen(theFile, "r");
    if (fp == NULL){
        printf("Could not open file %s",theFile);
        return 1;
    }

    while ((fgets(str, MAXCHAR, fp) != NULL)){
	theDistance = atoi(str);

	
	if(checkDistance(theDistance,counter,arrayOfDistances)==false){
		callEmail();
		break;
	}
	else{
		checkDistance(theDistance,counter,arrayOfDistances);
		counter++;
	}

	
}
    fclose(fp);
	return 0;
}


int writeCSV(const char statName[], const Statistics& stats){
	/*
	int size = sizeOfArray(filename);
	char file[size+5];
	copyArray(filename, file);
	file[size] = '.';
	file[size+1] = 't';
	file[size+2] = 'x';
	file[size+3] = 't';
	file[size+4] = 0;
	*/
	FILE *statFile;
	statFile =fopen(statName,"w");

	fprintf(statFile, "Minimum: %d \n" ,stats.minimum);
	fprintf(statFile, "Average: %f \n" ,stats.average);
	fprintf(statFile,  "Maximum: %d \n" , stats.maximum);
	fprintf(statFile,  "Population Standard Deviation: %f \n" , stats.popStdDev);
	fprintf(statFile,  "Sample Standard Deviation: %f \n" , stats.smplStdDev );

	fprintf(statFile,  "Modes: ");
	for(int x = 0; x < stats.mode.numModes - 1; x++) {
					fprintf(statFile, "%d ", stats.mode.modes[x]);
	}
	fprintf(statFile, "%d" ,stats.mode.modes[stats.mode.numModes -1]);



	fclose(statFile);
	return 0;
}	



int main() {

    	char* filename = "/dev/ttyS1";
	Dataset data = {0};
	Statistics stat = {0};
	readCSV("data.txt", data);


	computeStatistics(data, stat);

	writeCSV("out.txt", stat);
	
	
	//FILE *fopen(statName,w);
	//ifstream infile;
	/*ofstream outfile;
        outfile.open(statName);
        if(!outfile.is_open()) {
                return -1;
        }
        outfile <<"cheese";*/
	




    return 0;
}
