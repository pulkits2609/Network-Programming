//to process clients parallely , we need to use fork()
//using fork we can implement the following workflow : 
//parent process handles listening for new client connections
//each child process will be a new client that is connected, child handles communication
// Parent:
//     listen()
//     accept()
//     fork()
// Child:
//     communicate with client
//     close session
#include<iostream>
#include<unistd.h>

int main(){
    //fork is special because it is called once but returns twice
    pid_t pid = fork();
    //when we run this , the OS Creates a new process , now there are new processes and both continue execution from the next line
    if(pid == 0){
        //0 = child process
        std::cout<<"Child Process : pid : "<<getpid()<<" ppid : "<<getppid()<<"\n";
    }
    else if(pid > 0){
        // >0 = parent process
        std::cout<<"Parent Process : pid : "<<getpid()<<" ppid : "<<getppid()<<"\n";        
    }
    else{
        //error , failed to create child process
        perror("Failed to Create Child Process");
    }
    //the order of the if else block does not control the order of execution
    //the os scheduler decides which process runs first
    // When fork() is called, the original single process is duplicated by the OS, creating a new child process.
    // Both parent and child continue execution from the next line after fork(). The fork() call is special
    // because it returns twice: in the parent process it returns a value greater than 0 (the PID of the child),
    // and in the child process it returns 0. Each process has its own separate copy of memory (due to Copy-On-Write),
    // so the variable storing the return value (pid) exists independently in both processes but contains different values.
    // Based on this return value, we distinguish parent (pid > 0) from child (pid == 0). After fork(), both processes
    // execute concurrently and independently from that point onward.

    return 0;
}