#include <stdio.h>

unsigned int Fib( unsigned int n ) { 
    if ( n < 2 ) return 1; 

    return Fib(n-1) + Fib(n-2); 
}

int main() {
   // you will need to modify all parameters, etc.
   int n = 50;
   int i;
	
   printf("Fibbonacci of %d: " , n);
	
   for(i = 0; i<n; i++) {
      printf("%d ",Fib(i));            
   }
}



