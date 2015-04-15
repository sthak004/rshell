#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string>
using namespace std;


int main(int argc, char** argv){
        char *user = getlogin(); //syscall for getting username
        //perror check for getlogin()
        if(user == NULL){
                perror("getlogin");
                exit(1);
        }        
        char hostname[100]; //create a char array for the hostname
        if((gethostname(hostname, sizeof(hostname))) == -1){
        perror("hostname");
        exit(1);
        }              
   
   
        cout << user << '@' << hostname << '$' <<  endl; //output user and host
        return 0;
}
