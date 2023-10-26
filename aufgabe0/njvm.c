#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) { //argc counts the number of arguments, argv is an array of strings containing the arguments
    if (argc > 1) {    // argv[0] is the name of the program, argv[1] is the first argument, argv[2] the second, etc.
    for(int i=1; i<argc; i++){
        if (strcmp(argv[i], "--version") == 0) { // strcmp compares two strings, if they are equal it returns 0
            printf("Ninja Virtual Machine Version 1.0\n");
        } else if (strcmp(argv[i], "--help") == 0) {
            printf("Help: Ninja Virtual Machine\n");
            printf("Usage : ./njvm [OPTION] [option] ..\n");
            printf("Options:\n");
            printf("  --version  show version and exit\n");
            printf("  --help     show this help and exit\n");
        } else {
            printf("Unkown command line argument '%s', try './njvm --help' \n", argv[i]);
        }
    }
    } else {
        printf("Ninja Virtual Machine Started\n");
        printf("Ninja Virtual Machine Stopped\n");
    }

    return 0;
}