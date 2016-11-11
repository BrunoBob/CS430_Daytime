//General includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//Socket include
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>

#define DT_SERVER "129.6.15.28"
#define DT_PORT "13"

#define SERVER_PORT 7777
#define MAX_CONNECTION_QUEUE 1

#define MAX_DAYTIME_LENGTH 80
#define OTM '*'

typedef struct{
  int year;
  int month;
  int day;
  int hours;
  int minutes;
  int seconds;
} daytime;

typedef struct{
  int socket;
  int number;
  pthread_mutex_t* connectionMutex;
} Client;

int connect_to_serv();
void deconnect(int csocket);

char* readLine(int csocket);

daytime getTime(char* timeString);

int getData(int position, int size, char* string);

void launchServer();

void* newClient(void* arg);
