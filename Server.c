#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg){   //error function
perror(msg);                   //interprets the error number and outputs the error description(using stderr)
exit(1);
}

int main(int argc, char *argv[]){   //pass command line arguments(argc - count of total arguments(2)=file_name + port_no, argv - actual arguments)

if(argc<2){                         //check the user inputs the port_no
    fprintf(stderr,"Port not provided.Program terminated\n");
    exit(1);
}
int sockfd,newsockfd,portno,n;   //define file descriptor after creating socket, new file descriptor when connection establish and port_no
char buffer[255];                //define string called buffer to store messages

struct sockaddr_in serv_addr,cli_addr;  //define a structure for store server ip address and client ip address
socklen_t clilen;

sockfd=socket(AF_INET,SOCK_STREAM,0);  //create socket with parameters AF_INET, SOCK_STREAM and connection type as TCP(0)

if(sockfd<0){     //if socket function did not work properly outputs the error
    error("Error opening socket");
}
bzero((char *)&serv_addr,sizeof(serv_addr));
portno=atoi(argv[1]);        //get the port_no(using atoi function for convert String to integer)

serv_addr.sin_family=AF_INET;
serv_addr.sin_addr.s_addr=INADDR_ANY;
serv_addr.sin_port=htons(portno);

if(bind(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr))<0){
    error("Binding failed");
}

listen(sockfd, 5);     //listen function, which allows maximum five number of clients at a given time
clilen=sizeof(cli_addr);

newsockfd=accept(sockfd,(struct sockaddr *)&cli_addr,&clilen);   //for access the connection

if(newsockfd<0){    //check the new connection is accepted
    error("Error on accept");
}

while(1){       //for continuous execution of the sending and receiving input/output messages
    bzero(buffer,225);
    n=read(newsockfd,buffer,225);   //function for read the client messages
    if(n<0){
        error("Error on reading");
    }
    printf("Client : %s\n",buffer);   //display client messages

    bzero(buffer,255);
    fgets(buffer,255,stdin);     //for read bytes from stream
    n=write(newsockfd,buffer,strlen(buffer));  //sends the server message using write function
    if(n<0){
        error("Error on writing");
    }
    int i=strncmp("Bye",buffer,3);  //to get termination condition of the loop as "Bye"
    if(i==0){
        break;  //termination of the loop
    }
}
close(newsockfd);
close(sockfd);
return 0;
}


