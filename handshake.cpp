#include <iostream>
#include <mutex> // mutex
#include <condition_variable>
#include <fstream>
#include <thread>
#include <string> // string
#include <unistd.h> // getopt
#include <fstream> // ifstream
#include <vector> // vector
#include <stdlib.h> // exit
#include <chrono> // chrono time

#include <cstring>

using namespace std;
// include additional necessary headers

condition_variable query_wait;
condition_variable response_wait;
mutex m;


void query(int c) {
    // Should print: the print number (starting from 0), "SYN", and the three dots "..."
    unique_lock<mutex> lck(m);
    //lck.unlock();
    for(int i = 0; i < c; i++){
        /*
        wait(sem_request)
        print
        post(sem_response)
        */
        query_wait.wait(lck);
        cout << "[" << i << "] SYN ... ";
        
        response_wait.notify_one();
    }
    
    
}

void response(int count) {
    // Should print "ACK"
    unique_lock<mutex> lck(m);
    //lck.unlock();
    
    query_wait.notify_one();
    for(int i = 0; i < count; i++){
        
        response_wait.wait(lck);
        cout << "ACK" << endl;
        
        query_wait.notify_all();
    }
    
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: ./handshake <count>\n");
        exit(1);
    }

    /**
     * Steps to follow:
     * 1. Get the "count" from cmd args
     * 2. Create necessary synchronization primitive(s)
     * 3. Create two threads, one for "SYN" and the other for "ACK"
     * 4. Provide the threads with necessary args
     * 5. Update the "query" and "response" functions to synchronize the output
    */
   
	
    int count = atoi(argv[1]);
  
    //cout << argc << " " << count << endl;

    vector<thread> t_vec;

    
    

    

    t_vec.push_back(thread(query, count));
    t_vec.push_back(thread(response, count));

    t_vec.at(0).join();
    t_vec.at(1).join();
   
    return 0;
}