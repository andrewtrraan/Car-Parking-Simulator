#include <stdio.h>
#include <time.h>

const char * timestamp()
{
	time_t ltime; /* calendar time */
	ltime=time(NULL); /* get current cal time */
	printf("%s",asctime( localtime(&ltime) ) );
	return asctime(localtime(&ltime));
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

int main() {
    writeLog("log.txt", "FATAL", "Error: not working");
    return 0;
}
