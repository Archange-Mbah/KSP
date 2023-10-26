 //version 1 NVM
#include <stdio.h> // include the standard input/output library
#include <string.h>// include the string library

#define STACK_SIZE 1000 //define the stack size
#define PROGRAM_SIZE 100// define the program size in which the  instructions are saved
/* define all instructions*/
#define HALT 0  
#define PUSHC 1
#define ADD 2
#define SUB 3
#define MUL 4
#define DIV 5
#define MOD 6
#define RDINT 7
#define WRINT 8
#define RDCHR 9
#define WRCHR 10
int sp=0; //stack pointer
int ip=0; //instruction pointer
signed int stack[STACK_SIZE]; //stack
unsigned int programmsaver[]={
    HALT,PUSHC,SUB,MUL,DIV,MOD,RDINT,WRINT,RDCHR,WRCHR
} ;//programmsaver
 //programmsaver


void push(int x) {
printf("-[%4s]-> pushing [%d] onto stack @sp [%d]\n", __func__, x, sp);
stack[sp]=x;
printf("-[%4s]-> inc stack pointer [%d -> ", __func__, sp);
sp++;
printf("%d]\n", sp);
}

int pop(void) {
printf("-[%4s]-> dec stack pointer [%d -> ", __func__, sp);
sp--;
printf("%d]\n", sp);
int tmp = stack[sp];
printf("-[%4s]-> popping [%d] from stack @sp [%d]\n", __func__, tmp, sp);
return tmp;
}

void print_stack(void) {
printf("\n Stack\n");
printf(".-------+--------.\n");
for (int i=sp; i>=0; i--) {
if (i==sp)
printf("|sp->%3d| <empty>|\n", i);
else
printf("|%7d| %5d |\n", i, stack[i]);
}
printf("'-------+--------'\n\n");
}

int searchForInstruction(void) {
return programmsaver[ip];//the fetch function returns the instruction at the current instruction pointer
}


 void programmlister(void) {

for (int i = 0; i < sizeof(programmsaver) / sizeof(programmsaver[0]); i++) {
        unsigned int instruction = programmsaver[i];
        int opcode = instruction & 0xFFFFFF;
        int immediate = (instruction >> 24) & 0xFF;

        printf("Address %d: Opcode %d, Immediate %d\n", i, opcode, immediate);
    }
}


void executeProgramm(void) {
printf("-[%4s]-> executing programm\n", __func__);
while (1) {
unsigned int instruction = searchForInstruction();//the instruction is saved in the variable instruction
int opcode=instruction;//the opcode is saved in the variable opcode

switch (opcode) {
    case PUSHC:{
        ip++;//the instruction pointer is incremented to fetch the next instruction
        int value=programmsaver[ip];//the value is saved in the variable value
        push(value);//the value is pushed onto the stack
        break;
}    
    case ADD:{
        int b=pop();
        int a=pop();
        push(a+b);
        break;
}
     case SUB:{
        int b=pop();
        int a=pop();
        push(a-b);
        break;
}
    case MUL:{
        int b=pop();
        int a=pop();
        push(a*b);
        break;
}

   case DIV:{
        int b=pop();
        int a=pop();
        push(a/b);
        break;
}
    case MOD:{
        int b=pop();
        int a=pop();
        push(a%b);
        break;
}
    case RDINT:{
        int value;
        scanf("%d", &value);
        push(value);
        break;
}
    case WRINT:{
        int value=pop();
        printf("%d\n", value);
        break;
}
    case RDCHR:{
        char value;
        scanf("%c", &value);
        push(value);
        break;
}
    case WRCHR:{
        char value=pop();
        printf("%c\n", value);
        break;
}
    case HALT:{
        printf("-[%4s]-> halting programm\n", __func__);
        return;
}
    default:{
        printf("-[%4s]-> unkown instruction [%d]\n", __func__, opcode);
        return;
}
}  
}
}



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
        programmlister();
        executeProgramm();
        print_stack();
        printf("Ninja Virtual Machine Stopped\n");
    }

    return 0;
}