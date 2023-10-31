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
#define SIGN_EXTEND(i) ((i) & 0x00800000 ? (i) | 0xFF000000 : (i)) // 0x00800000 = 00000000000000000000100000000000i if the number is negative it will give 1 and if it is positive it will give 0

/*puschc << 24 ist eine LEFT SHIFT also 01000000000000 */

unsigned int prog1[]={
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
//unsigned int prog2[];
unsigned int prog2[]={
    (PUSHC <<24)|(IMMEDIATE(-2)),// IMMEDIATE(-2) = 111111
    (RDINT <<24),
    (MUL <<24),
    (PUSHC <<24)| IMMEDIATE(3),
    ((ADD <<24)),
    (WRINT <<24),
    (PUSHC << 24) | IMMEDIATE('\n'), // we can also include a charakter in the program directly
    (WRCHR <<24),
    (HALT <<24)
    };

unsigned int prog3[]={
    (RDCHR<<24),
    (WRINT<<24),
    (PUSHC<<24)|IMMEDIATE('\n'),
    (WRCHR<<24),
    (HALT<<24)
    };


int sp=0; // the stackpointer
int stack[MAXITEMS]; // the stack



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
void add(){
    int a=pop();
    int b=pop();
    int c=a+b;
    push(c);
}

void programLister(unsigned int program[], int size){
  int pc=0;
  while(pc<size){
    unsigned int instruction = program[pc]; // Extract the instruction
    unsigned int opcode = instruction >> 24; // Extract the opcode
    int argument = SIGN_EXTEND(IMMEDIATE(instruction)); //if the argument is negative it will be extended to 32 bit and if it is positive it will be extended to 24 bit

        
    switch(opcode){
         case HALT:
         printf("00%d:  HALT  %d\n", pc, argument);
            pc++;
            break;
         case PUSHC:
             printf("00%d: PUSHC  %d\n", pc, argument);
             pc++;
            break;
        case ADD:
            printf("00%d:  ADD  %d\n", pc, argument);
                pc++;
                break;  
        case SUB:
            printf("00%d:  SUB  %d\n", pc, argument);
                pc++;
                break;
        case MUL:
            printf("00%d:  MUL  %d\n", pc, argument);
                pc++;
                break;
        case DIV:
            printf("00%d:  DIV  %d\n", pc, argument);
                pc++;
                break;
        case MOD:
             printf("00%d:  MOD  %d\n", pc, argument);
                 pc++;
                 break;
        case RDINT:
             printf("00%d:  RDINT  %d\n", pc, argument);
                 pc++;
                 break;
        case WRINT:
             printf("00%d:  WRINT  %d\n", pc, argument);
                 pc++;
                 break; 
        case RDCHR:
             printf("00%d:  RDCHR  %d\n", pc, argument);
                 pc++;
                 break;
        case WRCHR:
             printf("00%d:  WRCHR  %d\n", pc, argument);
                 pc++;
                 break;                                                                            
    }
  }
       printf("28 \n");
       printf("Ninja Virtual Machine stopped  n");
  
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
void division() {
    int b = pop();
    int a = pop();
    
    if (b != 0) {
        int c = a / b;
        push(c);
    } else {
        printf("Error: Division by zero\n");
        // Hier könnten Sie geeignete Maßnahmen ergreifen, um den Fehler zu behandeln
    }
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
     if(argc==1){
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
            printf("  --prog1    run program 1\n");
            printf("  --prog2    run program 2\n");
            printf("  --version  show version and exit\n");
            printf("  --help     show this help and exit\n");
        }
        else if(strcmp(argv[1],"--prog1")==0){
             programLister(prog1, sizeof(prog1)/sizeof(prog1[0])); // sizeof(prog1)/sizeof(prog1[0]) returns the size of the array
             
        }
        else if(strcmp(argv[1],"--prog2")==0){
             programLister(prog2, sizeof(prog2)/sizeof(prog2[0])); // sizeof(prog1)/sizeof(prog1[0]) returns the size of the array
        }
        else if(strcmp(argv[1],"--prog3")==0){
                programLister(prog3, sizeof(prog3)/sizeof(prog3[0])); // sizeof(prog1)/sizeof(prog1[0]) returns the size of the array
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