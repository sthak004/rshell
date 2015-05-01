#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
using namespace std;

#define printVec(x) for(unsigned int i = 0; i < x.size(); i++) \
                        cout << x.at(i) << " "; \
                    cout << endl;

int main(int argc, char**argv){

    vector<char> flags;
    flags.push_back('a');
    flags.push_back('l');
    flags.push_back('R');
    printVec(flags);
    //bool isFlag_a = false; //bool to determine is the flag is -a
    //bool isFlag_l = false; //bool to determine is the flag is -l
    //bool isFlag_R = false; //bool to determine is the flag is -R

    if(argc <= 1){ //did you type any command?
        cerr << "Too few arguments." << endl;
        exit(1);
    }

    return 0;
}
