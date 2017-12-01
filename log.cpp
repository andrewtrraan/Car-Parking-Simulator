#include <iostream>
#include <stdio.h>
#include <fstream>
#include <time.h>
using namespace std;

int writeLog(const char filename[], const char severity[], const char description[]){
	ofstream outfile(filename, ios::app);
	//outfile.open(filename);
	if(!outfile.is_open()) {
		return -1;
	}

	char buff[20];
    struct tm *sTm;

    time_t now = time (0);
    sTm = localtime (&now);
	cout << buff << endl;

    strftime (buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", sTm);

	outfile << buff << " " << severity <<" " << description << endl;
	outfile.close();
	return 0;
}

int main() {
    writeLog("log.txt", "FATAL", "Error: not working");
    return 0;
}
