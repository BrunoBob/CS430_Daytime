#include "client.h"

int main(int argc, char* argv[]){
  char* buffer = NULL;
  int client_socket;
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr)); //Fill the struct with zeros

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = inet_addr(DT_SERVER);  //Give IP adress
  addr.sin_port = htons(atoi(DT_PORT)); //Give port

  client_socket = connect_to_serv(addr);

  buffer = readLine(client_socket);
  printf("%s\n",buffer );

  deconnect(client_socket);

  return 0;
}

int connect_to_serv(struct sockaddr_in addr){
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
