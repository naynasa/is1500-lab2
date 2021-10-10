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
///@param n - largest number to check for primality / all numbers 2 to n are checked for prime status
///@return an array 
void print_sieve(int n){
  bool sieves_prime[n*n+2]; //sieves_prime[i] == 1 om i är primtal annars == 0
  sieves_prime[0] = 0;
  sieves_prime[1] = 0;
  for (int i = 2; i <= n*n+1; i++){

      sieves_prime [i] = 1; 
  
  }
  //array_of_primes = [2,3,5,...]
  //j = 0
  //while(array_of_primes.length)
  //array_of_primes[j] = isprime(i)
  //j++

  //int sqrt_rounded_up_int = (int) sqrt(n) +1;
  int number_of_primes = 0;
  int i = 1;
  while(number_of_primes < n){   
    i++;
      if(sieves_prime[i] == 1){ // 1 indicates that its a prime
          number_of_primes++;
          
          for (int j = i*i; j <= n*n; j+=i){ //i^2, i^2+i, i^2+2i... - remove all multiples of found prime
                sieves_prime[j] = 0;
            }
      }
     
  
  }
  //printf("färdig");

    printf("%llu",i);


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


