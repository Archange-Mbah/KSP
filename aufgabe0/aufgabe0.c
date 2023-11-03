#include <stdio.h>
#include <stdlib.h>
#include <string.h> // to use the strcmp function
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
#define MAXITEMS 10
#define programSaver 100
#define IMMEDIATE(x) ((x) & 0x00FFFFFF) // die 24 bit werden auf 0 gesetzt zb Immediate(3) = 000000000000000000000000000011

/*puschc << 24 ist eine LEFT SHIFT also 01000000000000 */

unsigned int program[]={
    (PUSHC <<24)| IMMEDIATE(3),
    (PUSHC <<24)| IMMEDIATE(4),
    (ADD <<24),
    (PUSHC <<24)|IMMEDIATE(10),
    (PUSHC <<24)|IMMEDIATE(6),
    (SUB <<24),
    (MUL <<24),
    (WRINT <<24),
    (PUSHC <<24)|IMMEDIATE(10),
    (WRCHR <<24),
    (HALT <<24)

};

int sp=0; // the stackpointer
int stack[MAXITEMS]; // the stack



void push(int x) {
//printf("-[%4s]-> pushing [%d] onto stack @sp [%d]\n", __func__, x, sp);
stack[sp]=x;
//printf("-[%4s]-> inc stack pointer [%d -> ", __func__, sp);
sp++;
//printf("%d]\n", sp);
}
int pop(void) {
//printf("-[%4s]-> dec stack pointer [%d -> ", __func__, sp);
sp--;
//printf("%d]\n", sp);
int tmp = stack[sp];
//printf("-[%4s]-> popping [%d] from stack @sp [%d]\n", __func__, tmp, sp);
return tmp;
}
void add(){
    int a=pop();
    int b=pop();
    int c=a+b;
    push(c);
}
void sub(){
    int a=pop();
    int b=pop();
    int c=a-b;
    push(c);
}
void mul(){
    int a=pop();
    int b=pop();
    int c=a*b;
    push(c);
}
void divi(){
    int a=pop();
    int b=pop();
    int c=a/b;
    push(c);
}
void mod(){
    int a=pop();
    int b=pop();
    int c=a%b;
    push(c);
}
    void rdint(){
        int a;
        scanf("%d", &a);
        push(a);
    }
    void wrint(){
        int a=pop();
        printf("%d", a);
    }
    void rdchr(){
        char a;
        scanf("%c", &a);
        push(a);
    }
    void wrchr(){
        char a=pop();
        printf("%c", a);
    }
    void halt(){
        exit(0);
    }



/*int main (int argc, char *argv[]) {
int value;
print_stack();
push(5);
print_stack();
push(3);
print_stack();
value=pop();
print_stack();
return 0;
}
*/
int main( int argc, char *argv[]){ // arguments are stored in argv and argc stores the number of arguments
     if(argc<1){
        printf("Error: No program selected\n");
        return 1;
     }
     else if (argc>1){
        if(strcmp(argv[1],"--version")==0){ // Strcmp returns 0 if the strings are equal
            printf("Ninja Virtual Machine version 1 (compiled %s %s)\n", __DATE__, __TIME__);
        }
        else if(strcmp(argv[1],"--help")==0){ // Strcmp returns 0 if the strings are equal
            printf("Usage: ninja [options] [option]\n");
            printf("Options:\n");
            printf("  --version  show version and exit\n");
            printf("  --help     show this help and exit\n");
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