#include "server.h"

int main(int argc, char* argv[]){
  launchServer();

  return 0;
}

int connect_to_serv(){

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr)); //Fill the struct with zeros

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(DT_SERVER);  //Give IP adress
  addr.sin_port = htons(atoi(DT_PORT)); //Give port
  int csocket = socket(AF_INET,SOCK_STREAM, 0); //Create the socket descriptor

  if(csocket == -1){
    perror("ERROR : can't create socket\n");
    exit(1);
  }

  if(connect(csocket, (struct sockaddr *) &addr, sizeof(struct sockaddr)) == -1){//connect to the server
    perror("ERROR : can't connect to the server\n");
    exit(1);
  } else {
    printf("Successfully connected to server\n");
  }
  return csocket;
}

void deconnect(int csocket){
  shutdown(csocket, 0);
}

char* readLine(int csocket){
  char* buffer =  (char*) malloc(sizeof(char));
  char* daytime = (char*) malloc(sizeof(char)*MAX_DAYTIME_LENGTH);
  int index = 0;

  //test


  while(buffer[0] != OTM){ //while not OTM char
    if(recv(csocket, buffer, sizeof(char), 0) != 1){ //Read the next char
      perror("ERROR : can't recv the character");
      exit(1);
    }
    if(buffer[0] != OTM){ // If not OTM put it in the string
      daytime[index] = buffer[0];
      index++;
    }
  }

  //OTM received : time to shut down connection and print time

  daytime[index-1] = '\0'; //End the string with terminating character

  return daytime; //Return the daytime
}

daytime getTime(char* timeString){
  daytime currentTime;// = (daytime) malloc(sizeof(currentTime));

  currentTime.year = getData(7 ,2, timeString);
  currentTime.month = getData(10, 2, timeString);
  currentTime.day = getData(13, 2, timeString);
  currentTime.hours = getData(16, 2, timeString);
  currentTime.minutes = getData(19, 2, timeString);
  currentTime.seconds = getData(22, 2, timeString);

  return currentTime;
}

int getData(int position, int size, char* string){
  char* buffer = (char*)malloc(sizeof(char)*size);
  memcpy(buffer, string+position, size);
  return atoi(buffer);
}

void* newClient(void* arg){
  Client client = *((Client*) arg);

  pthread_mutex_unlock(client.connectionMutex); //Unlock the mutex so other client can connect

  //Update the time with NIST server
  char* timeString = NULL;
  daytime currentTime;
  int clientSocket;

  clientSocket = connect_to_serv();

  timeString = readLine(clientSocket);

  currentTime = getTime(timeString);

  deconnect(clientSocket);

  printf("Updated time : %d-%d-%d %d:%d:%d\n", currentTime.year, currentTime.month, currentTime.day, currentTime.hours, currentTime.minutes, currentTime.seconds);

  char* bufferRcv = (char*)malloc(1*sizeof(char));
  char* bufferSend = (char*)malloc(2*sizeof(char));

  if(recv(client.socket, bufferRcv, 1, 0) < 0){ //Read receving data from socket
    perror("Error : not receiving request from socket");
    exit(1);
  }

  switch (bufferRcv[0]) {
    case '0':
      sprintf(bufferSend, "%d", currentTime.year);
      printf("Year requested\n" );
      break;
    case '1':
      sprintf(bufferSend, "%d", currentTime.month);
      printf("Month requested\n" );
      break;
    case '2':
      sprintf(bufferSend, "%d", currentTime.day);
      printf("Day requested\n" );
      break;
    case '3':
      sprintf(bufferSend, "%d", currentTime.hours);
      printf("Hours requested\n" );
      break;
    case '4':
      sprintf(bufferSend, "%d", currentTime.minutes);
      printf("Minutes requested\n" );
      break;
    case '5':
      sprintf(bufferSend, "%d", currentTime.seconds);
      printf("Seconds requested\n" );
      break;
  }

  if(send(client.socket, bufferSend, 2, 0) == -1){
    perror("Error : not sending time to socket");
    exit(1);
  }

  sleep(1);

  printf("\nClient number %d disconnected\n", client.number);

  //Close the socket
  if (close(client.socket) == -1) {
    perror("Error : closing socket");
    exit(1);
  }

  pthread_exit(NULL);  //End of the thread
}

void launchServer(){
  int numberOfClient = 0;
  int serverSocket;                 // descriptor of server socket
  struct sockaddr_in serverAddress; // for naming the server's listening socket
  pthread_mutex_t connectionMutex;  //  mutex used for connection with a client

  //Mutex initialisation
  if(pthread_mutex_init(&connectionMutex, NULL) != 0){
    perror("Error : mutex initialisation");
    exit(1);
  }

  // create unnamed network socket for server to listen on
  if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Error : creating socket");
    exit(1);
  }

  // fills socket information
  serverAddress.sin_family      = AF_INET;           // accept IP addresses
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // accept any IP adress
  serverAddress.sin_port        = htons(SERVER_PORT);       // port to listen on

  // binding socket to a port
  if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1) {
    perror("Error : binding socket");
    exit(1);
  }

  printf("Server start on port %d\n", SERVER_PORT);

  // listening for client connection
  if (listen(serverSocket, MAX_CONNECTION_QUEUE) == -1) {
    perror("Error : listening on socket");
    exit(1);
  }

  // server loop
  while (1) {
    Client client;

    //give mutex address to client and lock it
    client.connectionMutex = &connectionMutex;
    pthread_mutex_lock(client.connectionMutex);

    // accept connection to client
    if ((client.socket = accept(serverSocket, NULL, NULL)) == -1) {
      perror("Error : accepting client");
    } else {
      numberOfClient++;
      client.number = numberOfClient;
      printf("\nAccepted client number %d\n", numberOfClient);

      //Create a new thread for the new client
      pthread_t clientThread;
      if(pthread_create(&clientThread, NULL, newClient, &client) != 0 ){
        perror("Error : Creation of thread");
        exit(1);
      }
    }
  }
}
