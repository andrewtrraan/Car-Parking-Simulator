#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;
int main()
{
    char buff[20];
    struct tm *sTm;
    time_t now = time (0);
    sTm = localtime (&now);
    strftime (buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", sTm);
    cout << buff << endl;

    // Call the Python script passing a filename argument.
    std::string command = "Email.py";
    system(command.c_str());
}

//Average, Min, Max, Median, Mode, PopStd, SmplStd, Struct
