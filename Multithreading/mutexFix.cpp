//here we will be using mutex to fix the race condition issue we faced previously
#include<iostream>
#include<thread>
#include<vector>
#include<mutex>
using namespace std;

int counter = 0;
//create a global mutex that all threads share ! 
mutex counterMutex;

void incrementFunction(){
    for(int i=0;i<1000000;i++){
        // counterMutex.lock();
        // counter++;
        // counterMutex.unlock();
        //the above code works but is dangerous
        //if an exception happens between lock and unlock , mutex can remain locked forever
        //DEADLOCK

        lock_guard<mutex> guard(counterMutex);
        counter++;
        //Lock guard ensures that mutex is always released even if function exits early
    }
}   
int main(){
    vector<thread> workers{};
    for(int i=0;i<20;i++){
        workers.push_back(thread(incrementFunction));
    }
    for(int i=0;i<20;i++){
        workers[i].join(); 
    }

    cout<<"Final Counter Value : "<<counter<<"\n";
    return 0;
}