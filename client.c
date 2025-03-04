#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


void error(const char *msg){  //error function
perror(msg);   //interprets the error number and outputs the error description(using stdrr)
exit(0);
}

int main(int argc, char *argv[]){ //pass command line arguments(argc - count of total arguments(3)=file_name + server ip address + port_no, argv - actual arguments)

    int sockfd,portno,n;    //define socket,port number and integer variable n
    struct sockaddr_in serv_addr;
    struct hostent *server;    //for store the information of given host(host name,IPV4 address)

    char buffer[255];         //define string called buffer to store messages
    if(argc<3){               //check the user inputs the necessary inputs
        fprintf(stderr,"Usage %s hostname port\n",argv[0]);
        exit(1);
    }

    portno=atoi(argv[2]);    //get the port_no(using atoi function for convert String to integer)
    sockfd=socket(AF_INET,SOCK_STREAM,0);     //create socket with parameters AF_INET, SOCK_STREAM and connection type as TCP(0)
    if(sockfd<0){
        error("Error opening socket");
    }

    server=gethostbyname(argv[1]);   //get the ip address
    if(server==NULL){
        fprintf(stderr,"Error, No such host");
    }

    bzero((char *)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    bcopy((char *)server->h_addr,(char*)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port=htons(portno);

    if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0){    //connect function for connect to the server
        error("Connection failed");
    }

    while(1){       //for continuous execution of the sending and receiving input/output messages
        bzero(buffer,255);
        fgets(buffer,255,stdin);     //for read bytes from stream
        n=write(sockfd,buffer,strlen(buffer));    //sends the client message using write function
        if(n<0){
            error("Error on writing");
        }
        bzero(buffer,255);
        n=read(sockfd,buffer,255);     //function for read the server messages
        if(n<0){
            error("Error on reading");
        }
        printf("Server: %s",buffer);   //display server messages

        int i=strncmp("Bye",buffer,3);       //to get termination condition of the loop as "Bye"
        if(i==0){
            break;            //termination of the loop
        }

    }
    close(sockfd);    //close the client program
    return 0;
}


