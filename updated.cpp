#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <fstream>
#include <time.h>
#include <string.h>
#include <curl/curl.h>

#include <ugpio/ugpio.h>
#define MAXCHAR 1000

#define FROM    "garagesensor150@gmail.com"
#define TO      "abdum.shk@gmail.com"
#define CC      "pyrohamburger@gmail.com"

static const char *payload_text[] = {
  "Date: Mon, 29 Nov 2010 21:54:29 +1100\r\n",
  "To: " TO "\r\n",
  "From: " FROM "(Garage Sensor)\r\n",
  "Cc: " CC "(Another example User)\r\n",
  "Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@"
  "rfcpedant.example.org>\r\n",
  "Subject: Test Email\r\n",
  "\r\n", /* empty line to divide headers from body, see RFC5322 */
  "Hello World\r\n",
  "\r\n",
  NULL
};

struct upload_status {
  int lines_read;
};

static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp)
{
  struct upload_status *upload_ctx = (struct upload_status *)userp;
  const char *data;

  if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
    return 0;
  }

  data = payload_text[upload_ctx->lines_read];

  if(data) {
    size_t len = strlen(data);
    memcpy(ptr, data, len);
    upload_ctx->lines_read++;

    return len;
  }

  return 0;
}

int main(void)
{
  CURL *curl;
  CURLcode res = CURLE_OK;
  struct curl_slist *recipients = NULL;
  struct upload_status upload_ctx;

  upload_ctx.lines_read = 0;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
    curl_easy_setopt(curl, CURLOPT_USERNAME, "garagesensor150@gmail.com");
    curl_easy_setopt(curl, CURLOPT_PASSWORD, "paulward");
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM);

    recipients = curl_slist_append(recipients, TO);
    recipients = curl_slist_append(recipients, CC);
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    res = curl_easy_perform(curl);

    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    curl_slist_free_all(recipients);

    curl_easy_cleanup(curl);
  }

  return (int)res;
}








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
