//////////////////////////////////////////////////////////////
//
// Your #includes here; make sure you are allowed them ...
//
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <math.h>
using namespace std;
//////////////////////////////////////////////////////////////
//
// struct definitions
//

#ifndef MARMOSET_TESTING
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
#endif

//////////////////////////////////////////////////////////////
//
// Function declarations; do not remove
//
int readCSV(const char filename[],const int minAcceptableID,
				const int maxAcceptableID,Dataset& data,
				Rejects& rejects);

int computeStatistics(Dataset& data, Statistics& stats);

int writeCSV(const char filename[], const Statistics& stats);

//////////////////////////////////////////////////////////////
//
// Function definitions. PLEASE USE HELPER FUNCTIONS
//
bool noExtension = true;
int sizeOfArray(const char arr[]){
	int num = 0;
	int i = 0;
	while(arr[i] != 0){
		if(arr[i] == '.'){
			noExtension = false;
			break;
		}
		num++;
		i++;
	}
	return num;
}
void copyArray(const char arr[], char dest[]){
	int i = 0;
	while(arr[i] != 0){
		dest[i] = arr[i];
		i++;
	}
}

int readCSV(const char filename[],Dataset& data){
	if(filename == NULL)
			return -1;

	int size = sizeOfArray(filename);
	char file[size+5];
	copyArray(filename, file);
	file[size] = '.';
	if(noExtension){
		file[size + 1] = 'c';
		file[size + 2] = 's';
		file[size + 3] = 'v';
		file[size + 4] = 0;
	}
	else{
		for(int i = 0; i < size; i++){ //Check for .csv extension
			if(file[i] == '.'){
					int j = i + 1;
					if(size < j+3 || size > j+3){
						return -1;
					}
					else if(file[j] == 'c' && file[j+1] == 's' && file[j+2] == 'v'){
						break;
					}
					else{
						return -1;
					}
			}
		}
	}

	data.car = new DayCar[100];
	data.numDays = 0;
	int k = 0; //index for student array
	const int maxLineLength = 100;
	char line[maxLineLength];
	ifstream infile; // declare the file object
	infile.open(filename); // open the file
	if (!infile.is_open()){
		return -1; // Unable to open file
	}
	bool done = false;
	int fileLineNumber = 0;
	while (!done) { // Read from file
		++fileLineNumber;
		if (!infile.getline(line, maxLineLength)) { // Get next line
			if (infile.eof()) { // End of file
				done = true;
			}
			else { // Some error reading file
				return -1;
			}
		}
		if(done)
			break;
		int i = 0;
		int numEnEx = 0; //track number of enter/exits
		while(line[i] != 0){
			if(line[i] >= '0' && line[i] <= '9'){
				numEnEx = numEnEx * 10 + line[i] - 48;
				i++;
			}
			else if(line[i] == ' ' || line[i] == '\t'){
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
return 0;
}


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

int writeCSV(const char filename[], const Statistics& stats){
	int size = sizeOfArray(filename);
	char file[size+5];
	copyArray(filename, file);
	file[size] = '.';
	file[size+1] = 't';
	file[size+2] = 'x';
	file[size+3] = 't';
	file[size+4] = 0;
	ofstream outfile;
	outfile.open(file);
	if(!outfile.is_open()) {
					return -1;
	}

	outfile << "Minimum: " << stats.minimum << endl;
	outfile << "Average: " << stats.average << endl;
	outfile << "Maximum: " << stats.maximum << endl;
	outfile << "Population Standard Deviation: " << stats.popStdDev << endl;
	outfile << "Sample Standard Deviation: " << stats.smplStdDev << endl;

	outfile << "Modes: ";
	for(int x = 0; x < stats.mode.numModes - 1; x++) {
					outfile << stats.mode.modes[x] << ", ";
	}
	outfile << stats.mode.modes[stats.mode.numModes -1] << endl;

	outfile << "Histogram: " << endl;
	outfile << "[0-9]: " << stats.histogram[0] << endl;
	outfile << "[10-19]: " << stats.histogram[1] << endl;
	outfile << "[20-29]: " << stats.histogram[2] << endl;
	outfile << "[30-39]: " << stats.histogram[3] << endl;
	outfile << "[40-49]: " << stats.histogram[4] << endl;
	outfile << "[50-59]: " << stats.histogram[5] << endl;
	outfile << "[60-69]: " << stats.histogram[6] << endl;
	outfile << "[70-79]: " << stats.histogram[7] << endl;
	outfile << "[80-89]: " << stats.histogram[8] << endl;
	outfile << "[90-100]: " << stats.histogram[9] << endl;


	outfile.close();
	return 0;
}
//////////////////////////////////////////////////////////////
//
// Test Driver

// Do not remove the #ifndef and its associated #endif
//

#ifndef MARMOSET_TESTING

#define isNaN(X) (X != X)  // NaN is the only float that is not equal to itself

int main(const int argc, const char* const argv[]) {

	// Some test driver code here ....
	int minAcceptableID = 0;
	int maxAcceptableID = 2000000;

	Dataset data = {0};
	Rejects rejects = {0};
	Statistics stats = {0};

	std::cout << std::endl << "Implement some more appropriate tests in main()" << std::endl << std::endl;

	if(readCSV("data.txt", data) < 0)
	{
		std::cout << ".csv file could not be read" << std::endl;
	}
	cout << data.numDays << endl;
	if (computeStatistics(data, stats) < 0)
		{
			std::cout << "Stats could not be computed" << std::endl;
		}

		if (writeCSV("Statistics", stats) < 0)
		{
			std::cout << ".stat file could not be written" << std::endl;
		}
	std::cout << "Average: " << stats.average << std::endl;
	std::cout << "Minimum: " << stats.minimum << std::endl;
	std::cout << "Maximum: " << stats.maximum << std::endl;
	std::cout << "Population standard deviation: " << stats.popStdDev << std::endl;
	std::cout << "Sample standard deviation: " << stats.smplStdDev << std::endl;


	for (unsigned char i = 0; i < stats.mode.numModes; i++)
	{
		std::cout << "Mode: " << stats.mode.modes[i] << std::endl;
	}

	for (unsigned char i = 0; i < 10; i++)
	{
		std::cout << "Histogram bin [" << (i*10) << ", " << ((((i+1)*10) - 1) + i/9) << "]: " << stats.histogram[i] << std::endl;
	}


	return 0;
}

#endif
