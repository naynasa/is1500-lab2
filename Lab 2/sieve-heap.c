/*
 print-prime.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define COLUMNS 6



/**
 * Returns 1 if n is prime else 0.
 * @param n - the number to be checked for prime status
 * @return - 1 or 0 (true or false) whether or not n is prime
*/
int is_prime(int n){
  if(n<2){
    return 0;
  }
  int sqrt_rounded_up_int = (int) sqrt(n) +1;
  for(int i = 2;i<sqrt_rounded_up_int;i++){
    if(n % i == 0){
      return 0;
    }
  }
  return 1;
}
void print_number(int number_to_print){
  printf("%8d ", number_to_print);
}
///does not use more than n bytes of the array (n+12 bytes total)
///@param n - largest number to check for primality / all numbers 2 to n are checked for prime status
///@return an array 
void print_sieve(int n){
  // int *x
  //mem[x] -> y
    //int* pointer_to_array = (int *) malloc(sizeof(int)*(n-1));
    //bool is 1 byte
    bool* sieves_prime = (bool *) malloc(sizeof(bool)*(n+1)); //sieves_prime[i] == 1 om i är primtal annars == 0 - n+1 eftersom vi har med 0 också därmed blir det n+1 värden
    //example n==5
    //[0,1,2,3,4,5] i.e. sieves_prime[0] <=> is_prime(2)
    //[0,0,0,0,0,0]
    for (int i = 0; i <= 2; i++){//initialize number 0 and 1 to false
       sieves_prime [i] = false; //syntactic sugar for *(sieves_prime+i) = 0;
   }
   for (int i = 2; i <= n; i++){
       sieves_prime [i] = true; // *(sieves_prime+i) = 1;
   }
   
    int sqrt_rounded_up_int = (int) sqrt(n) +1;
   for  (int i = 2; i <= sqrt_rounded_up_int; i++){ 
       if(sieves_prime[i] == 1){ // 1 indicates that its a prime *(sieves_prime+i) == 1;
           for (int j = i*i; j <= n; j+=i){//i^2, i^2+i, i^2+2i...
           sieves_prime[j] = 0; // *(sieves_prime+j) = 0;

           }
        }
    
   }
   int num_prints_since_last_newline = 0;
   for (int i = 0; i <= n; i++){

       if(sieves_prime[i] == 1){
           print_number(i);
           num_prints_since_last_newline +=1;
       }
       if(num_prints_since_last_newline >= COLUMNS){
        printf("\n");
        num_prints_since_last_newline = 0;
       }
    }
    //printf("%8d",&sieves_prime);
    free(sieves_prime);

   }

   /*
     int num_prints_since_last_newline = 0;
  for(int i = 2; i < n; i++){
    if(is_prime(i)){
      print_number(i);
      num_prints_since_last_newline +=1;
    }
    if(num_prints_since_last_newline >= COLUMNS){
      printf("\n");
      num_prints_since_last_newline = 0;
    }
  }
  */
  

    
    //printf("%d", *sieves_prime);
    



// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
  if(argc == 1+1) //1+ num_arguments in the program
    print_sieve(atoi(argv[1]));
  else
    printf("Please state an interger number.\n");
  return 0;


}
//2e7 till 5e7 give or take sieves-heap
//sieve går snabbare (stacken snabbare än heapen)

