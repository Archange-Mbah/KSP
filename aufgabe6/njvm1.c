#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> // to use the strcmp function
#include "bigint.h"// to use the bigInt functions
#include "support.h"// to use the fatalError function
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
#define VERSION 6
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
#define CALL 26
#define RET 27
#define DROP 28
#define PUSHR 29
#define POPR 30
#define DUP 31

// here we have the prototypes of the functions
void push(int x);
int pop(void);
void pushg(int x);
void popg(int x);
void asf(int x);
void rsf(void);
void pushl(int x);
void popl(int x);
void sub(void);
void mul(void);
void division(void);
void mod(void);
void rdint(void);
void wrint(void);
void rdchr(void);
void wrchr(void);
void halt(void);
void add(void);
void eq(void);
void ne(void);
void lt(void);
void le(void);
void gt(void);
void ge(void);
void jmp(int x);
void brf(int x);
void brt(int x);
void call(int x);
void ret(void);
void drop(int n);
void pushr(void);
void popr(void);
void dup(void);
void executeInstruction(unsigned int opcode, int argument);
void instructionLister(unsigned int opcode, int argument);
void programLister(void);
void debugMode(void);
void programExecutor(void);

typedef struct {
    unsigned int size;
    unsigned char data[1];
}
 Object;
typedef Object *ObjRef;
typedef struct{
     bool isObjRef;
     union{
        ObjRef objref;
        int intVal;
     }u;
} StackSlot;

StackSlot stack[MAXITEMS]; // the stack





/*puschc << 24 ist eine LEFT SHIFT also 01000000000000 */

bool haltbool = false;
bool debug=false;
bool breakpoint=false;

int sp=0; // the stackpointer
int pc=0;
int fp=0; // the framepointer
 
int *program_memory; // the program memory
ObjRef *data_memory;
int breakpointVariable=0;
ObjRef return_register=0;

void * getPrimObjectDataPointer(void * obj){
    ObjRef oo = ((ObjRef)  (obj));
    return oo->data;
}

void fatalError(char *msg) {
    printf("Fatal error: %s\n", msg);
    exit(1);
}

void * newPrimObject(int dataSize) {
    ObjRef bigObjRef;

    bigObjRef = malloc(sizeof(unsigned int) +
            dataSize * sizeof(unsigned char)) ;
    if (bigObjRef == NULL) {
        fatalError("newPrimObject() got no memory");
    }
    bigObjRef->size = dataSize;
    return bigObjRef;
}

//unsigned int prog2[];

ObjRef createObjRef(int value){ // create an object reference

ObjRef intObject;
 unsigned int objSize=sizeof(unsigned int)+sizeof(int);
    intObject=malloc(objSize);
    intObject->size=sizeof(int);
    *(int *)intObject->data=value;
    return intObject;

}

//leg ein Number Array an und speichere die Werte in diesem Array
void pushNumber(int x) {
stack[sp].isObjRef=false;
stack[sp].u.intVal=x;
sp++;
}

void pushObjRef(int x) {
stack[sp].isObjRef=true;
ObjRef newObj=createObjRef(x);
stack[sp].u.objref=newObj;
sp++;
}
int popNumber(void) {
sp--;
//printf("-[%4s]-> dec stack pointer %d -> ", __func__, sp);
return stack[sp].u.intVal;
}
ObjRef popObjRef(void) {
sp--;
//printf("-[%4s]-> dec stack pointer %d -> ", __func__, sp);
return stack[sp].u.objref;
}
 void pushg(int x){  // push the value of the global variable x onto the stack
 stack[sp].u.objref=data_memory[x];
    sp++;
}
 void popg(int x){ // pop the value from the stack and store it in the global variable x
    data_memory[x]=popObjRef();
 }

void asf(int x){ // allocate stack frame
        pushNumber(fp); // push the old frame pointer onto the stack
        fp=sp; // set the frame pointer to the current stack pointer
        sp=sp+x; // increment the stack pointer by x
    }
void rsf(void){ // restore stack frame
        sp=fp; // set the stack pointer to the frame pointer
        fp=popNumber(); // pop the old frame pointer from the stack and store it in the frame pointer
    }
    void pushl(int x){ // push the value o stack[sp].u.objref=stack[fp+x].u.objref;
    stack[sp].u.objref=stack[fp+x].u.objref;
    sp++;
            
    }
    void popl(int x){ // pop the value from the stack and store it in the local variable x
        stack[fp+x].u.objref=popObjRef();
    }	    
 
// the stack
void sub(void){
int a=*(int *)popObjRef()->data;
int b=*(int *)popObjRef()->data;
int c;
c=b-a;
pushObjRef(c);

}

void mul(void) {
     int a= *(int *)popObjRef()->data;
     int b= *(int *)popObjRef()->data;
     int c=a*b;
     pushObjRef(c);
}

void division(void) {
       int a= *(int *)popObjRef()->data;
       int b= *(int *)popObjRef()->data;
       int c; // we have to allocate space in the heap for the object
              if(a!=0){
                c=b/a;
                pushObjRef(c);
              }
              else{
                printf("Error: Division by zero\n");
                exit(1);
              }
}
    
void mod(void) {
    int a= *(int *)popObjRef()->data;
    int b= *(int *)popObjRef()->data;
    int c;
    if(a!=0){
        c=b%a;
        pushObjRef(c);
    }
    else{
        printf("Error: Division by zero\n");
        exit(1);
    }
}
       
void rdint(void){
        int a;
        scanf("%d", &a);
        pushObjRef(a);
        
    }
    void wrint(void){
      int a=*(int *)popObjRef()->data;
        printf("%d", a);
    }
    void rdchr(void){
        char a;
        scanf("%c", &a);
        pushObjRef(a);
    }
    void wrchr(void){
        char a=*(int *)popObjRef()->data;
        printf("%c", a);
    }
    void halt(void){
        haltbool = true;
    }



void add(void){
    int a=*(int *)popObjRef()->data; // *(int *)popObjRef()->data is the same as *(int *)popObjRef()->data
    int b=*(int *)popObjRef()->data; // *(int *)popObjRef()->data is the same as *(int *)popObjRef()->data
    int c;
    c=a+b;
     pushObjRef(c);   
}

void eq(void){
    int a=*(int *)popObjRef()->data;
    int b=*(int *)popObjRef()->data;
     int c;
    if(a==b){
        c=1;
    }
    else{
        c=0;
    }
    pushObjRef(c);
}
void ne(void){
    int a=*(int *)popObjRef()->data;
    int b=*(int *)popObjRef()->data;
    int c;
    if(a!=b){
        c=1;
    }
    else{
        c=0;
    }
    pushObjRef(c);
}
void lt(void){
    int a=*(int *)popObjRef()->data;
    int b=*(int *)popObjRef()->data;
    int c;
    if(b<a){
        c=1;
    }
    else{
        c=0;
    }
    pushObjRef(c);
}
void le(void){
    int a=*(int *)popObjRef()->data;
    int b=*(int *)popObjRef()->data;
    int c;
    if(b<=a){
        c=1;
    }
    else{
        c=0;
    }
    pushObjRef(c);
}
void gt(void){
    int a=*(int *)popObjRef()->data;
    int b=*(int *)popObjRef()->data;
    int c;
    if(b>a){
        c=1;
    }
    else{
        c=0;

    }
    pushObjRef(c);
}
void ge(void){
    int a=*(int *)popObjRef()->data;
    int b=*(int *)popObjRef()->data;
    int c;
    if(b>=a){
        c=1;
    }
    else{
        c=0;
    }
    pushObjRef(c);
}
void jmp(int x){
    pc=x;
}
void brf(int x){
    int  a=*(int *)popObjRef()->data;
    if((a)==0){
        jmp(x);
    }
}
void brt(int x){
   int  a=*(int *)popObjRef()->data;
    if((a)!=0){
        jmp(x);
    }
}
void call(int x){
    pushObjRef(pc);
    jmp(x);
}
void ret(void){
   int  a=*(int *)popObjRef()->data;
    int b=a;
    jmp(b);
}
void drop(int n){
    while(n>0){
        popObjRef();
        n--;
       
    }
}

void pushr(void){
    stack[sp].u.objref=return_register;
    sp++;
     
}
void popr(void){
    return_register=popObjRef();
}
void dup(void){
    stack[sp].u.objref=stack[sp-1].u.objref;
    sp++;
}




void executeInstruction(unsigned int opcode ,int argument){
     switch(opcode){
            case HALT:
                halt();
                
                break;
            case PUSHC:
                pushObjRef(argument);
                
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
            case CALL:
              call(argument);
                   break;
            case RET:   
                ret();
                break;   
            case DROP:
                drop(argument);
                break;  
            case PUSHR:
                pushr();
                break;
            case POPR:
                popr();
                break; 
            case DUP:
                dup();
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
        case CALL:                                                               
                printf("00%d:\tcall %d\n", pc-1, argument);
                    break;
        case RET:                                                               
                printf("00%d:\tret\n", pc-1);
                    break; 
        case DROP:                                                               
                printf("00%d:\tdrop %d\n", pc-1, argument);
                    break;
        case PUSHR:                                                               
                printf("00%d:\tpushr\n", pc-1);
                    break;
        case POPR:                                                               
                printf("00%d:\tpopr\n", pc-1);
                    break; 
        case DUP:                                                               
                printf("00%d:\tdup\n", pc-1);
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
  
void programExecutor(void){
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

                    
//int read_len=0;
      
      if(argc==1 ||( argc==2 && (strcmp(argv[1],"--debug")==0))){
        printf("Error: No program selected\n");
        return 1;
     }
     
     for(int i=1;i<argc;i++){
        if(strcmp	(argv[i],"--version")==0){
            printf("Ninja Virtual Machine version %d\n compiled on %s at %s\n", VERSION, __DATE__, __TIME__);
            exit(0);    
        }
      if(strcmp(argv[1],"--help")==0){ // Strcmp returns 0 if the strings are equal
            printf("Usage: ninja [options] [option]\n");
            printf("Options:\n");
            printf("  --version  show version and exit\n");
            printf("  --help     show this help and exit\n");
            printf("  --debug    start ninja in debug mode\n");
          exit(0);
        }
    
       if(strstr(argv[i],".bin")!=NULL){
                     x=i;
                     break;
        }
       
} 

     
    
     

    if( (fp=fopen(argv[x], "rb"))==NULL){
              printf("Error: cannot open code file %s\n", argv[x]);
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

        for(int i =1; i<argc; i++){
       
         
        if(strcmp(argv[i],"--debug")==0){
            debug=true;
            printf("debug an\n");
     }
       
        }
       
        int instructionCount;
        fread(&instructionCount, 4, 1, fp);
        program_memory=malloc(instructionCount*sizeof(int)); // allocate memory for the program instructions
        int numberOfVariables;
        fread(&numberOfVariables, 4, 1, fp);
        data_memory=malloc(numberOfVariables*sizeof(ObjRef)); //allocate memory for the data memory it is an array of pointers to objects
         
         fread(program_memory, sizeof(int), instructionCount, fp);
            printf("Ninja Virtual Machine started\n");
            programExecutor();
            printf("Ninja Virtual Machine stopped\n");
            fclose(fp);
}
   


