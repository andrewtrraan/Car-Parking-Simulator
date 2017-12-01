//#include <iostream>
#include <time.h>
//using namespace std;

void timestamp()
{
	time_t ltime; /* calendar time */
	ltime=time(NULL); /* get current cal time */
	printf("%s",asctime( localtime(&ltime) ) );
}

int main() {
		timestamp();
    return 0;
}
