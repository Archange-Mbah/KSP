#include <stdio.h>
#include <stdlib.h>
#include <string.h> // to use the strcmp function
#define PUSCHC

int main( int argc, char *argv[]){ // arguments are stored in argv and argc stores the number of arguments
     if(argc>1){
        if(strcmp(argv[1],"--version")==0){ // Strcmp returns 0 if the strings are equal
            printf("Ninja Virtual Machine version 0 (compiled %s %s)\n", __DATE__, __TIME__);
        }
        else if(strcmp(argv[1],"--help")==0){ // Strcmp returns 0 if the strings are equal
            printf("Usage: ninja [options] [option]\n");
            printf("Options:\n");
            printf("--prog1      select program 1 to execute\n");
            printf("--prog2      select program 2 to execute\n");
            printf("--prog3      select program 3 to execute\n");
            printf("--version  show version and exit\n");
            printf("--help     show this help and exit\n");
        }
        else{
            printf("Unknown command line argument %s\n, try './njvm --help' ", argv[1]);
            
        }

     }
    else{    
            printf("Ninja VIrtual Machine started\n");
              printf("Ninja Virtual Machine stopped\n");
    }
    return 0;


}