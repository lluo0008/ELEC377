
#include <stdio.h>

struct {
    char buffer[100];
    void (*x)();
} y;

void foo();
int main(){

    char *exploit =
     
     "xxxxxxxxxxxxxxxxxxxx"
     "xxxxxxxxxxxxxxxxxxxx"
     "xxxxxxxxxxxxxxxxxxxx"
	 "xxxxxxxxxxxxxxxxxxxx"
	 "xxxxxxxxxxxxxxxxxxxx"
	 "WXYZ";

    char exploit2[10] = {
    // exploit code goes here
        
    // buffer address goes here
    
    // NULL terminator
    0x00

    };

    int i;

    y.x = foo;
    for (i = 0; exploit[i]; i++){
	y.buffer[i] = exploit[i];
    }

    (*y.x)();
}


void foo(){
   printf("%s %x\n", y.buffer, &y.x);
}
