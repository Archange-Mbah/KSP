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
#define VERSION 3
#define PUSHG 11    
#define POPG 12
#define ASF 13
#define RSF 14
#define PUSHL 15
#define POPL 16
#define EQ 17
#define NE 18
#define LT 19
#define LE 20
#define GT 21
#define GE 22
#define JMP 23
#define BRF 24
#define BRT 25


/*puschc << 24 ist eine LEFT SHIFT also 01000000000000 */

bool haltbool = false;
bool debug=false;
bool breakpoint=false;

int sp=0; // the stackpointer
int stack[MAXITEMS];
int pc=0;
int fp=0; // the framepointer
 
int *program_memory; // the program memory
int *data_memory;
int breakpointVariable=0;

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
 void pushg(int x){  // push the value of the global variable x onto the stack
        int value=data_memory[x];
        push(value);
 }
 void popg(int x){ // pop the value from the stack and store it in the global variable x
     int value=pop();
     data_memory[x]=value;
 }
    void asf(int x){ // allocate stack frame
        push(fp); // push the old frame pointer onto the stack
        fp=sp; // set the frame pointer to the current stack pointer
        sp=sp+x; // increment the stack pointer by the size of the local variables
    }
    void rsf(){ // restore stack frame
        sp=fp; // set the stack pointer to the frame pointer
        fp=pop(); // pop the old frame pointer from the stack
    }
    void pushl(int x){ // push the value of the local variable x onto the stack
       int value=stack[fp+x];
       push(value);
    }
    void popl(int x){ // pop the value from the stack and store it in the local variable x
        int value=pop();
        stack[fp+x]=value;
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
        exit(1);
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



void add(){
    int a=pop();
    int b=pop();
    int c=a+b;
    push(c);
}

void eq(){
    int a=pop();
    int b=pop();
    if(a==b){
        push(1);
    }
    else{
        push(0);
    }
}
void ne(){
    int a=pop();
    int b=pop();
    if(a!=b){
        push(1);
    }
    else{
        push(0);
    }
}
void lt(){
    int a=pop();
    int b=pop();
    if(b<a){
        push(1);
    }
    else{
        push(0);
    }
}
void le(){
    int a=pop();
    int b=pop();
    if(b<=a){
        push(1);
    }
    else{
        push(0);
    }
}
void gt(){
    int a=pop();
    int b=pop();
    if(b>a){
        push(1);
    }
    else{
        push(0);
    }
}
void ge(){
    int a=pop();
    int b=pop();
    if(b>=a){
        push(1);
    }
    else{
        push(0);
    }
}
void jmp(int x){
    pc=x;
}

void brf(int x){
    int a=pop();
    if(a==0){
        jmp(x);
    }
}
void brt(int x){
    int a=pop();
    if(a==1){
        jmp(x);
    }
}

void executeInstruction(unsigned int opcode ,int argument){
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
            case PUSHG:
                pushg(argument);
                break; 
            case POPG:
                popg(argument);
                break;
            case ASF:
                asf(argument);
                break;
            case RSF:
                rsf();
                break;
            case PUSHL:
                pushl(argument);
                break;
            case POPL:
                popl(argument);
                break; 
            case EQ:
                eq();
                break;
            case NE:
                ne();
                break;
            case LT:
                lt();
                break;
            case LE:
                le();
                break;
            case GT:
                gt();
                break;
            case GE:    
                ge();
                break; 
            case JMP:
                jmp(argument);
                break;  
            case BRF:    
                brf(argument);
                break;
            case BRT:    
                brt(argument);
                break;                             
        }

}  

   void instructionLister(unsigned int opcode ,int argument){
    switch(opcode){
         case HALT:
         printf("00%d:\thalt\n", pc-1);
         halt();
            break;
         case PUSHC:
             printf("00%d:\tpushc  %d\n", pc-1, argument);
            break;
        case ADD:
            printf("00%d:\tadd\n", pc-1);
                break;  
        case SUB:
            printf("00%d:\tsub\n", pc-1);
                break;
        case MUL:
            printf("00%d:\tmul\n", pc-1);
                
                break;
        case DIV:
            printf("00%d:\tdiv\n", pc-1);
                break;
        case MOD:
             printf("00%d:\tmod\n", pc-1);
                 break;
        case RDINT:
             printf("00%d:\trdint\n", pc-1);
                 break;
        case WRINT:
             printf("00%d:\twrint\n", pc-1);
                 break; 
        case RDCHR:
             printf("00%d:\trdchr\n", pc-1);
                 break;
        case WRCHR:
             printf("00%d:\twrchr\n", pc-1);
                 
                 break;   
        case PUSHG:
             printf("00%d:\tpushg  %d\n", pc-1, argument);
                 break;
        case POPG:  
                printf("00%d:\tpopg %d\n", pc-1, argument);
                    break;      
        case ASF:   
                printf("00%d:\tasf %d\n", pc-1, argument);
                    break;
        case RSF:                                                                   
                printf("00%d:\trsf\n", pc-1);
                    break;
        case PUSHL:                                                                   
                printf("00%d:\tpushl %d\n", pc-1, argument);
                    break;
        case POPL:                                                                   
                printf("00%d:\tpopl %d\n", pc, argument);
                    break;
        case EQ:                                                               
                printf("00%d:\teq\n", pc-1);
                    break;
        case NE:
                   printf("00%d:\tne\n", pc-1);
                        break;
        case LT:                                                               
                printf("00%d:\tlt\n", pc-1);
                    break;
        case LE:                                                               
                printf("00%d:\tle\n", pc-1);
                    break;
        case GT:                                                               
                printf("00%d:\tgt\n", pc);
                    break;
        case GE:                                                               
                printf("00%d:\tge\n", pc-1);
                    break;
        case JMP:                                                               
                printf("00%d:\tjmp %d\n", pc-1, argument);
                    break;
        case BRF:                                                               
                printf("00%d:\tbrf %d\n", pc-1, argument);
                    break;
        case BRT:                                                               
                printf("00%d:\tbrt %d\n", pc-1, argument);
                    break;
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
    instructionLister(opcode,argument);
   }
}

  void debugMode(){
    
     printf("DEBUG: inspect, list, breakpoint, step, run, quit?");
    char c[50];
    while(debug){
         printf("debug> ");
        scanf("%s",c);
         if(strcmp(c,"breakpoint")==0){
              scanf("%d",&breakpointVariable);
              printf("breakpoint set at  %d\n",breakpointVariable);
              breakpoint=true;
              
            } 
        else if(strcmp(c,"run")==0){
            if(pc==breakpointVariable){
                break;
            }
             debug=false; 
             printf("debug aus\n");   
        }
        else if(strcmp(c,"inspect")==0){
           scanf("%s",c);
            if(strcmp(c,"stack")==0)printf("stack");
             else if(strcmp(c,"data")==0) printf("data");
            else printf("repeat");
        }
        else if(strcmp(c,"quit") == 0){
            exit(0);
        } 
        else if(strcmp(c,"step")==0){
            break;
        } 
        else if(strcmp(c,"list")==0){
            programLister();
        }  
       

        else{
            printf("DEBUG: inspect, list, breakpoint, step, run, quit?");
        }
  }
  }
  
void programExecutor(){
    pc = 0;
    haltbool = false;
    while(!haltbool){  //   question: avant on sauvegardait les instructions dans un tableau et on les executait apres mais maintenant on les execute directement, comment est ce que les instuctions sont mnt lues?
        unsigned int instruction = program_memory[pc]; // Extract the instruction
        pc++;
        unsigned int opcode = instruction >> 24; // Extract the opcode
        int argument = SIGN_EXTEND(instruction & 0x00FFFFFF); //if the argument is negative it will be extended to 32 bit and if it is positive it will be extended to 24 bit

        if(debug) {
            instructionLister(opcode,argument);
            debugMode();
           
        }

    
        executeInstruction(opcode,argument);

        if(breakpoint && pc==breakpointVariable){
            debug=true;
            }
        }

 }

int main( int argc, char *argv[]){ // arguments are stored in argv and argc stores the number of arguments
FILE *fp=NULL;
int x;
int d=0;
int h=0;
int v=0;

         
         
        for(int i =1; i<argc; i++){
            if(strcmp(argv[i], "--version")==0)v=1;
             if (strcmp(argv[i], "--help")==0)h=1;
            if (strcmp(argv[i], "--debug")==0)d=1;
         }
    
         if(v+h+d<argc-2 ){
             printf("Error: Unknown option , try --help\n");
             exit(1);
        }
        if(v==1){
            printf("Ninja Virtual Machine version 2 (compiled %s %s)\n", __DATE__, __TIME__);
            exit(0);
        }
                    
//int read_len=0;
     for(int i =1; i<argc; i++){
        if(strcmp(argv[i],"--debug")==0){
            debug=true;
            printf("debug an\n");
        }
     }
     for(int i=1;i<argc;i++){
       if(strstr(argv[i],".bin")!=NULL){
         x=i;
         break;
       }
       else{

       }
     }
     if(argc==1){
        printf("Error: No program selected\n");
        return 1;
     }
     else if (argc>1){
        if(strcmp(argv[1],"--version")==0){ // Strcmp returns 0 if the strings are equal
            printf("Ninja Virtual Machine version 2 (compiled %s %s)\n", __DATE__, __TIME__);
        }
        else if(strcmp(argv[1],"--help")==0){ // Strcmp returns 0 if the strings are equal
            printf("Usage: ninja [options] [option]\n");
            printf("Options:\n");
            printf("  --version  show version and exit\n");
            printf("  --help     show this help and exit\n");
        }
       
        
        else {
            printf("Ninja Virtual Machine started  \n");
           
         if( (fp=fopen(argv[x], "rb"))==NULL){
              printf("Error: Could not open file\n");
              exit(1);
              }
            
          char c[4];
          fread(c, 1, 4, fp);// we copy the first 4 bytes of the file into c and fp icrements by 1
          if(strncmp(c, "NJBF", 4)!=0){
              printf("Error: Wrong file format\n");
              exit(1);
          }
        int version;
        fread(&version, 4, 1, fp);// fread returns the number of elements successfully read
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
            programExecutor();
            printf("Ninja Virtual Machine stopped  \n");
            fclose(fp);
}
   }
}
