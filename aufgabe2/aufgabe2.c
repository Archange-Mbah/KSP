#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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
#define MAXITEMS 10000
#define programSaver 100
#define IMMEDIATE(x) ((x) & 0x00FFFFFF) // die 24 bit werden auf 0 gesetzt zb Immediate(3) = 000000000000000000000000000011
#define SIGN_EXTEND(i) ((i) & 0x00800000 ? (i) | 0xFF000000 : (i)) // 0x00800000 = 00000000000000000000100000000000i if the number is negative it will give 1 and if it is positive it will give 0
#define VERSION 2
#define PUSHG 11    
#define POPG 12
#define ASF 13
#define RSF 14

/*puschc << 24 ist eine LEFT SHIFT also 01000000000000 */

bool haltbool = false;

int sp=0; // the stackpointer
int stack[MAXITEMS];
int pc=0;

int *program_memory;
int *data_memory;

//unsigned int prog2[];




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

// the stack
void sub(){
    int a=pop();
    int b=pop();
    int c=b-a;
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
    int c=b%a;
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
        haltbool = true;
    }

/*void print_stack(void) {
printf("\n Stack\n");




//printf(".-------+--------.\n");
for (int i=sp; i>=0; i--) {
if (i==sp)
printf("|sp->%3d| <empty>|\n", i);
else
printf("|%7d| %5d |\n", i, stack[i]);
}
printf("'-------+--------'\n\n");
}
*/

void add(){
    int a=pop();
    int b=pop();
    int c=a+b;
    push(c);
}

void programExecutor(){
    pc = 0;
    haltbool = false;
    while(!haltbool){
        unsigned int instruction = program_memory[pc]; // Extract the instruction
        pc++;
        unsigned int opcode = instruction >> 24; // Extract the opcode
        int argument = SIGN_EXTEND(instruction & 0x00FFFFFF); //if the argument is negative it will be extended to 32 bit and if it is positive it will be extended to 24 bit

        
        switch(opcode){
            case HALT:
                halt();
                
                break;
            case PUSHC:
                push(argument);
                
                break;
            case ADD:
                add();
               
                break;  
            case SUB:
                sub();
                
                break;
            case MUL:
                mul();
                
                break;
            case DIV:
                division();
                
                break;
            case MOD:
                mod();
               
                break;
            case RDINT:
                rdint();
               
                break;
            case WRINT:
                wrint();
               
                break; 
            case RDCHR:
                rdchr();
            
                break;
            case WRCHR:
                wrchr();
                break;                                                                            
        }
    
    }

 }
void programLister(){
    haltbool = false;
  pc=0;
  while(!haltbool){
    unsigned int instruction = program_memory[pc]; // Extract the instruction
    pc++;
    unsigned int opcode = instruction >> 24; // Extract the opcode
    int argument = SIGN_EXTEND(instruction & 0x00FFFFFF); //if the argument is negative it will be extended to 32 bit and if it is positive it will be extended to 24 bit

        
    switch(opcode){
         case HALT:
         printf("00%d:\tHALT\n", pc);
         halt();
            break;
         case PUSHC:
             printf("00%d:\tPUSHC  %d\n", pc, argument);
            break;
        case ADD:
            printf("00%d:\tADD\n", pc);
                break;  
        case SUB:
            printf("00%d:\tSUB\n", pc);
                break;
        case MUL:
            printf("00%d:\tMUL\n", pc);
                
                break;
        case DIV:
            printf("00%d:\tDIV\n", pc);
                break;
        case MOD:
             printf("00%d:\tMOD\n", pc);
                 break;
        case RDINT:
             printf("00%d:\tRDINT\n", pc);
                 break;
        case WRINT:
             printf("00%d:\tWRINT\n", pc);
                 break; 
        case RDCHR:
             printf("00%d:\tRDCHR\n", pc);
                 break;
        case WRCHR:
             printf("00%d:\tWRCHR\n", pc);
                 
                 break;                                                                            
    }
  }
       
       
  
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
FILE *fp=NULL;
//int read_len=0;
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
            printf("  --version  show version and exit\n");
            printf("  --help     show this help and exit\n");
        }
        else {
            printf("Ninja Virtual Machine started  \n");
          if((fp=fopen(argv[1], "rb"))==NULL){
              printf("Error: Could not open file\n");
              exit(1);
          }
          char c[4];
          fread(c, 1, 4, fp);
          if(strncmp(c, "NJBF", 4)!=0){
              printf("Error: Wrong file format\n");
              exit(1);
          }
        int version;
        fread(&version, 4, 1, fp);
        if(version!=VERSION){
            printf("Error: Wrong file version\n");
            exit(1);
        }
        int instructionCount;
        fread(&instructionCount, 4, 1, fp);
        program_memory=malloc(instructionCount*sizeof(int)); // allocate memory for the program instructions
        int numberOfVariables;
        fread(&numberOfVariables, 4, 1, fp);
        data_memory=malloc(numberOfVariables*sizeof(int)); // allocate memory for the data variables
         
         fread(program_memory, sizeof(int), instructionCount, fp);
        
          
              
              
            programLister();
            programExecutor();
            printf("Ninja Virtual Machine stopped  \n");
            fclose(fp);

          
             
          
             
        }

     }
   


}