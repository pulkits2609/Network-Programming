//now we will be beginning by creating the client socket first
//client only requires a few system calls which are socket() and connect() system calls
#include<iostream>
#include<sys/types.h> //this is required to be present before socket.h in some systems
#include<sys/socket.h> //required for socket system calls
#include<string.h>
#include<sys/un.h> //for sockaddr_un structure
#include<unistd.h> //for write and read system call
int main(){
    //initialize the buffer
    char rbuf[30];
    //initialize socket descriptor before creating socket
    int sockfd = -1;
    sockfd = socket(AF_UNIX,SOCK_STREAM,0); //AF_UNIX is the Local Socket Family
    //SOCK_STREAM is for connection based sockets , 0 is the default protocol followed by SOCK_STREAM
    if(sockfd < 0){
        std::cout<<"Socket Creation Failed \n";
        //if socket creation fails , socket() returns -1
        return -1;
    }
    std::cout<<"Socket Creation Success\n";
    //after socket has been created we need to connect it to the server using its address
    struct sockaddr_un serv; //initialize socket address
    serv.sun_family = AF_UNIX; //specify the socket addres family
    strcpy(serv.sun_path,"./LocalSocketFile"); //provide with the server socket file path
    
    int ret=-1;
    ret = connect(sockfd,(struct sockaddr *)&serv,sizeof(serv));
    if(ret < 0){
        std::cout<<"Connection Failed\n";
        close(sockfd);
        return -1;
    }
    std::cout<<"Connection to Server Established ! \n";
    //writing hi to server
    write(sockfd,"Hi",3);
    std::cout<<"Data Written to server \n";
    //reading data from server
    read(sockfd,rbuf,sizeof(rbuf)); //read only those bytes that the buffer can accomodate
    std::cout<<"Data Read from the server is : "<<rbuf<<"\n";
    close(sockfd);
    std::cout<<"Client Connection Terminated\n";
    //when this is closed , server will also automatically close their connection
    return 0;
}
