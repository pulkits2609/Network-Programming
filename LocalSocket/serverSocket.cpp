#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/un.h>
#include<unistd.h>
#include<string.h>
int main(){
    //initialize socket file descriptor
    int sockfd = -1;
    //initialize socket session descriptor
    int sessfd = -1;
    char buf[20]; //buffer where server stores and reads data from client
    char wbuf[] = "Hello from Server";

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0); //create a local socket
    if (sockfd < 0){ //check for successful creation
        std::cout << "Socket Creation Failed\n";
        return -1; 
    }
    std::cout << "Socket Creation Success\n";

    //creating server socket address
    struct sockaddr_un serv;
    serv.sun_family = AF_UNIX;
    strcpy(serv.sun_path, "./LocalSock"); 

    // Remove existing socket file if it exists
    unlink("./LocalSock");
    // Bind Socket
    if (bind(sockfd, (struct sockaddr*)&serv, sizeof(serv)) < 0) { //in case binding fails
        std::cout << "Bind Failed\n";
        close(sockfd);
        return -1;
    }
    std::cout << "Bind SUCCESS\n";
    // start listening for connect requests from client
    listen(sockfd, 5);
    std::cout << "Listening...\n";

    struct sockaddr_un cli;
    socklen_t len;

    while(1){ //server will handle 1 client at a time , but when the client disconnects , server will keep running

        len = sizeof(cli); // MUST reset before every accept

        sessfd = accept(sockfd, (struct sockaddr*)&cli, &len);
        if (sessfd < 0) {
            std::cout << "Accept failed\n";
            continue; // do not close listening socket
        }
        std::cout << "Client connection accepted\n";

        memset(buf, 0, sizeof(buf)); // clear buffer before read

        read(sessfd, buf, sizeof(buf)); //read from client
        std::cout << "SERVER::: Data read from client is " << buf << "\n";

        write(sessfd, wbuf, sizeof(wbuf)); //write to client
        std::cout << "SERVER::: Data written to client\n";

        close(sessfd); //close the session socket
    }
    close(sockfd); //close the main socket
    // Remove the socket file
    unlink("./LocalSock");
    return 0;
}
