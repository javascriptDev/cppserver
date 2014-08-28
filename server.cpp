#include <iostream>
#include <cstdio>
#include <netinet/in.h> ///for sockaddr_in
#include <sys/types.h>  //for socket
#include <unistd.h> //for close socket
#include <sys/socket.h>//for socket
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for bzero
#include <time.h> // for time_t and time
#include <string.h>


#define SERVER_PORT 7754
#define QUEUE_LENGTH 20
#define BUFFER_SIZE 1024

using namespace std;
int main(int argc,char **argv){


struct sockaddr_in server_addr;
bzero(&server_addr,sizeof(server_addr));
server_addr.sin_family=AF_INET;
server_addr.sin_addr.s_addr=htons(INADDR_ANY);
server_addr.sin_port=htons(SERVER_PORT);

FILE *stream;

int server_socket=socket(AF_INET,SOCK_STREAM,0);
  printf("init socket begin\n");
  if(server_socket<0){
	printf("create socket failed");
	exit(1);
  }
//printf("init socket success\n");
  if(bind(server_socket,(struct sockaddr*)&server_addr,sizeof(server_addr))){
	printf("server bind port %d failed",SERVER_PORT);
	exit(1);
  }
//printf("bind success\n");  
if(listen(server_socket,QUEUE_LENGTH)){
	printf("listen failed");
	exit(1);
  }	
  printf("listen in port 7754\n");
  
  while(1){
	struct sockaddr_in client_addr;
	socklen_t length=sizeof(client_addr);
	int new_server_socket=accept(server_socket,(struct sockaddr*)&client_addr,&length);
	if(new_server_socket<0){
		printf("server accept failed\n");
		break;
	}
	printf("accept a client request\n");
	char buffer[BUFFER_SIZE];
	bzero(buffer,BUFFER_SIZE);
	length=recv(new_server_socket,buffer,BUFFER_SIZE,0);
	if(length<0){
		printf("server recieve data failed\n");
		exit(1);
	}
	//print the request header
      //   printf("%s",strtok(buffer,"\n"));
	char *p;
	char *url;	
	char *fileType;
	p = strtok(buffer,"\n"); 
	strtok(p," ");
	//get the first line of buffer
	url = strtok(NULL," ");
        url = strtok(url,"/");
  	string s = url;
	fileType = strtok(url,".");    
	fileType = strtok(NULL,".");

	printf("requtst url is %s",s.c_str());
	printf("file type is %s",fileType);
		

//	printf("\n%s",buffer);

	if((stream=fopen(s.c_str(),"r"))==NULL){
		printf("the file was not opened \n");
		exit(1);
	}else{
	//	printf("file was opened \n");
	}
	bzero(buffer,BUFFER_SIZE);

	int lengthsize=0;
/*
	while(lengthsize=fread(buffer,10,1024,stream)>0){
		printf("%d",lengthsize);
	//	printf("lengthsize= %d\n",lengthsize);
		if(send(new_server_socket,buffer,lengthsize,0)<0){
			printf("send file is failed!\n");
			break;
		}
		bzero(buffer,BUFFER_SIZE);
	}
*/
	lengthsize=fread(buffer,10,1024,stream);

	char result[1024];
	const char *header="HTTP:/1.1 200 OK\r\nContent-type:text/html\r\nContent-Length:%d\r\n\r\n%s";
	sprintf(result,header,strlen(buffer),buffer);
	if(send(new_server_socket,result,strlen(result),0)<0){
		printf("send file failed!\n");
		continue;
	}
//	printf("file readed!\n");	

	if(fclose(stream)){
		printf("the file data was not closed!\n");
		exit(1);
	}
//	printf("file is closed!\n");

	close(new_server_socket);
//	printf("close new_server_socket \n");

  }
	close(server_socket);
//	printf("close server_socket\n");
	return 0;
}

