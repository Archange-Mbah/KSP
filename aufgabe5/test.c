#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> // to use the strcmp function
#define MAXITEMS 10000

typedef int Object;
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




//unsigned int prog2[];

ObjRef createObjRef(int value){ // create an object reference
    Object obj=value;
    ObjRef objref=malloc(sizeof(Object));
    *objref=obj;
    return objref;
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
printf("pushed  reference  to%d\n", x);
sp++;
}
int popNumber(void) {
sp--;
//printf("-[%4s]-> dec stack pointer %d -> ", __func__, sp);
return stack[sp].u.intVal;
}
ObjRef popObjRef(void) {
sp--;
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
        printf("sp:%d\n",sp);
}
    void rsf(void){ // restore stack frame
        sp=fp; // set the stack pointer to the frame pointer
        fp=popNumber(); // pop the old frame pointer from the stack and store it in the frame pointer
        printf("sp:%d\n",sp);
    }
void pushl(int x) {
    stack[sp].u.objref=stack[fp+x].u.objref;
    sp++;
    printf("sp:%d\n",sp);
}

void popl(int x) {
   stack[fp+x].u.objref=popObjRef();
}
// the stack
void sub(void){
    int a= *(popObjRef());
    int b= *(popObjRef());
    int c;
    c=b-a;
    pushObjRef(c);

}
/*int b=*(popObjRef());
int c;
c=b-a;
pushObjRef(c);*/


void mul(void) {
     int a= *(popObjRef());
     int b= *(popObjRef());
    int c;
            c=a*b;
     pushObjRef(c);
}

void division(void) {
       int a= *(popObjRef());
       int b= *(popObjRef());
              int c;// we have to allocate space in the heap for the object
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
    int a= *(popObjRef());
    int b= *(popObjRef());
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
    Object a=*(popObjRef());
    printf("%d\n", a);
    }
    void rdchr(void){
        char a;
        scanf("%c", &a);
        pushObjRef(a);
    }
    void wrchr(void){
        char a=*(popObjRef());
        printf("%c\n", a);
    }
    void halt(void){
        haltbool = true;
    }



void add(void){
    int a=*popObjRef();
    int b=*popObjRef();
    int c;
    c=a+b;
     pushObjRef(c);   
}

void eq(void){
    int a=*popObjRef();
    int b=*popObjRef();
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
    int a=*popObjRef();
    int b=*popObjRef();
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
    int a=*popObjRef();
    int b=*popObjRef();
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
    int a=*popObjRef();
    int b=*popObjRef();
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
    int a=*popObjRef();
    int b=*popObjRef();
   int c;
    if(b>a){
        c=1;
    }
    else{
        c=0;

    }
}
void ge(void){
    int a=*popObjRef();
    int b=*popObjRef();
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
    ObjRef a=popObjRef();
    if((*a)==0){
        jmp(x);
    }
}
void brt(int x){
    ObjRef a=popObjRef();
    if((*a)!=0){
        jmp(x);
    }
}
void call(int x){
    pushObjRef(pc);
    jmp(x);
}
void ret(void){
    ObjRef a=popObjRef();
     jmp(*a);
}
void drop(int n){
    while(n>0){
        if(stack[sp-1].isObjRef==true){
            popObjRef();
            }
        else{
            popNumber();
        }
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

int main( int argc, char *argv[]){
   asf(3);
   printf("fp:%d\n",fp);
   printf("sp:%d\n",sp);
   pushObjRef(11);
printf("fp:%d\n",fp);
printf("sp:%d\n",sp);
   pushObjRef(33);
    printf("fp:%d\n",fp);
    printf("sp:%d\n",sp);
    pushObjRef(1);
   asf(2);
    printf("fp:%d\n",fp);
    printf("sp:%d\n",sp);
   pushl(-4);
   printf("fp:%d\n",fp);
   wrint();
   pushObjRef(10);
   wrchr();
    pushObjRef(22);
    wrint();
    pushObjRef(10);
    wrchr();
    pushl(-3);
    wrint();
    pushObjRef(10);
    wrchr();
    rsf();
    printf("fp:%d\n",fp);
    ret();
    drop(2);
    rsf();
    printf("fp:%d\n",fp);
      
    /*
    ObjRef a=popObjRef();
    printf("the value of a is");
    printf("%d",*a);
    pushObjRef(33);
    ObjRef b=popObjRef();
    printf("the value of b is");
    printf("%d",*b);*/

    
}